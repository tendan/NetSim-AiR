#include "factory.hpp"
#include "nodes.hpp"
#include <istream>
#include <string>
#include <sstream>

bool has_reachable_storehouse(PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Sender does not have any receivers");
    }

    for (const auto& receiver : sender->receiver_preferences_.get_preferences()) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            return true;
        } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            PackageSender* sendrecv_ptr = dynamic_cast<PackageSender*>(dynamic_cast<class Worker*>(receiver.first));

            if (sendrecv_ptr == sender) {
                continue;
            }

            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED && has_reachable_storehouse(sendrecv_ptr, node_colors)) {
                return true;
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;
    throw std::logic_error("Error");
}

void Factory::remove_worker(ElementID id){
    Worker* node = &(*cont_w.find_by_id(id));
    std::for_each(cont_r.begin(), cont_r.end(), [&node](Ramp& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    cont_w.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id)
{
    Storehouse* node = &(*cont_s.find_by_id(id));
    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    cont_s.remove_by_id(id);
}

void Factory::do_deliveries(Time t) {
    for (auto &ramp : cont_r)
        ramp.deliver_goods(t);
}

void Factory::do_work(Time t ) {
    for (auto& worker : cont_w)
        worker.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &ramp : cont_r)
        ramp.send_package();

    for (auto &worker : cont_w)
        worker.send_package();
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> kolor;

    auto set_unvisited_colors = [&kolor](const auto& container) {
        for (const auto& item : container) {
            auto sender = dynamic_cast<PackageSender*>(&item);
            kolor[sender] = NodeColor::UNVISITED;
        }
    };

    set_unvisited_colors(cont_w);
    set_unvisited_colors(cont_r);

    try {
        for (const auto& ramp : cont_r) {
            auto sender = dynamic_cast<PackageSender*>(&ramp);
            has_reachable_storehouse(sender, kolor);
        }
    } catch (const std::logic_error&) {
        return false;
    }

    return true;
}

std::map<std::string, ElementType> element_types{
        {"LOADING_RAMP", ElementType::RAMP},
        {"WORKER", ElementType::WORKER},
        {"STOREHOUSE", ElementType::STOREHOUSE},
        {"LINK", ElementType::LINK},
};

ParsedLineData parse_line(std::string& line) {
    // rozłóż linię na ID określający typ elementu oraz tokeny "klucz=wartość"
    // każdy token rozłóż na parę (klucz, wartość)
    // na podstawie ID oraz par (klucz, wartość) zwróć odpowiedni obiekt typu ParsedLineData
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    //  Schemat wstępnego przetwarzania pojedynczej linii (rodzaju analizy składniowej – sprawdzenia zgodności z gramatyką języka) jest identyczny dla każdego typu elementu sieci (dla każdego typu węzła i dla połączenia)
    //    sprawdź, czy linia zaczyna się od jednego z predefiniowanych znaczników (np. LINK); jeśli nie – rzuć wyjątek
    //    wyodrębnij występujące po znaczniku pary klucz-wartość (w postaci “klucz=wartość”, rozdzielone spacjami)
    //    i zachowaj je w kontenerze umożliwiającym ich wygodną analizę (np. w postaci mapy);
    //    zwróć uwagę, że na tym etapie wszystkie dane występujące pomiędzy znakiem “=” a spacją powinny być potraktowane jako wartość
    //    – zatem wartość skojarzona z kluczem będzie łańcuchem znaków (np. typu std::string)


    while (std::getline(token_stream, token, delimiter))
        tokens.push_back(token);

    ParsedLineData parsed_data;

    try {
        parsed_data.element_type = element_types.at(tokens[0]);

        std::for_each(std::next(tokens.cbegin()), tokens.cend(), [&](const std::string& parameter_str) {
            std::stringstream parameter_stream(parameter_str);
            std::string part;
            std::vector<std::string> key_value;
            while(std::getline(parameter_stream, part, '=')) {
                key_value.push_back(part);
            }
            parsed_data.parameters[key_value[0]] = key_value[1];
        });
    } catch (std::out_of_range& ex) {
        throw std::exception();
    }

    return parsed_data;
}

Factory load_factory_structure(std::istream& is) {
    // utwórz (pusty) obiekt typu Factory
    //
    // dla każdej linii w pliku
    //      jeśli linia pusta lub rozpoczyna się od znaku komentarza - przejdź do kolejnej linii
    //      dokonaj parsowania linii
    //      w zależności od typu elementu - wykorzystaj pary (klucz, wartość) do poprawnego:
    //       * zainicjalizowania obiektu właściwego typu węzła i dodania go do obiektu fabryki, albo
    //       * utworzenia połączenia między zadanymi węzłami sieci
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty() || line[0] == ';')
            continue;
        ParsedLineData parsed = parse_line(line);

        switch(parsed.element_type) {
            case RAMP:
                std::cout << "Wczytano rampę" << std::endl;
                break;
            case WORKER:
                std::cout << "Wczytano pracownika" << std::endl;
                break;
            case STOREHOUSE:
                std::cout << "Wczytano magazyn" << std::endl;
                break;
            case LINK:
                std::cout << "Wczytano połączenie" << std::endl;
                break;
        }
    }

    return {};
}

//void save_factory_structure(Factory& factory, std::ostream& os) {
//    // TODO?
//}