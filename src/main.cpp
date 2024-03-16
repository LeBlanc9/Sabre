#include <iostream>
#include <sabre_layout.h>
#include <sabre.h>


CouplingList get_default_clist() {
    CouplingList c_list = {
        std::make_tuple(0, 1, 0.95),
        std::make_tuple(1, 0, 0.95),
        std::make_tuple(1, 2, 0.99),
        std::make_tuple(2, 3, 0.96),
        std::make_tuple(2, 1, 0.99),
        std::make_tuple(3, 2, 0.96),
        std::make_tuple(3, 4, 0.9),
        std::make_tuple(4, 3, 0.9)
    };
    return c_list;
}


int main() {
    std::cout << "-- main function --" << std::endl;

    // CouplingList c_list = get_default_clist();
    // Backend backend(c_list);
    // Model model;

    SabreLayout sabreLayout;
}
