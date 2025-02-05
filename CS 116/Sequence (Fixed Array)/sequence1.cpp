#include <cstdlib>  // Provides size_t
#include "sequence1.h"
using namespace std;

namespace main_savitch_3
{
    sequence::sequence() {
        current_index = 0;
        used = 0;
    }

    void sequence::start() {
        current_index = 0;
    }

    void sequence::advance() {
        current_index++;
    }

    void sequence::insert(const value_type& entry) {
        if (!is_item()) {
            current_index = 0;
        }
        for (int i = used; i > current_index; i--) {
            data[i] = data[i - 1];
        }
        data[current_index] = entry;
        used++;
    }
    
    void sequence::attach(const value_type& entry) {
        if (!is_item()) {
            current_index = used;
        }
        else {
            current_index++;
        }
        for (int i = used; i > current_index; i--) {
            data[i] = data[i - 1];
        }
        data[current_index] = entry;
        used++;

    }

    void sequence::remove_current() {
        for (int i = current_index; i < used - 1; i++) {
            data[i] = data[i + 1];
        }
        used--;
    }

    size_t sequence::size() const {
        return used;
    }

    bool sequence::is_item() const {
        return current_index < used;
    }

    double sequence::current() const {
        return data[current_index];
    }
}