#include "sequence2.h"
#include<iostream>
using namespace std;

namespace main_savitch_4 {
	sequence::sequence(size_type initial_capacity) {
		capacity = initial_capacity;
		data = new value_type[capacity];
		used = 0;
        current_index = 0;
	}

	sequence::sequence(const sequence& source) {
        data = new value_type[source.capacity];
        copy(source.data, source.data + source.used, data);
        if(source.is_item())
            current_index = source.current_index;
        used = source.used;
        capacity = source.capacity;
	}

	sequence::~sequence() {
		delete[] data;
	}

	void sequence::resize(size_type new_capacity) {
        double* new_data;
        if (new_capacity == capacity) {
            return;
        }
        if (new_capacity < used) {
            new_capacity = used;
        }
        capacity = new_capacity;
        new_data = new value_type[capacity];
        copy(data, data + used, new_data);
        delete[] data;
        data = new_data;
	}

    void sequence::start() {
        current_index = 0;
    }

    void sequence::advance() {
        current_index++;
    }

    void sequence::insert(const value_type& entry) {
        size_type i;
        if (!is_item()) {
            current_index = 0;
        }
        for (i = used; i > current_index; i--) {
            data[i] = data[i - 1];
        }
        data[current_index] = entry;
        used++;
    }

    void sequence::attach(const value_type& entry) {
        size_type i;
        if (!is_item()) {
            current_index = used;
        }
        else {
            current_index++;
        }
        for (i = used; i > current_index; i--) {
            data[i] = data[i - 1];
        }
        data[current_index] = entry;
        used++;

    }

    void sequence::remove_current() {
        size_type i; 
        for (i = current_index; i < used - 1; i++) {
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

    void sequence::operator =(const sequence& source) {
        value_type* new_data;
        
        if (this == &source) {
            return;
        }
        
        if (capacity != source.capacity)
        {
            new_data = new value_type[source.capacity];
            delete[] data;
            data = new_data;
            capacity = source.capacity;
        }
        current_index = source.current_index;
        used = source.used;
        copy(source.data, source.data + source.used, data);
    }

    void sequence::operator +=(const sequence& source) {
        if (capacity < used + source.used) {
            resize(used + source.used);
        }
        copy(source.data, source.data + source.used, data + used);
        used += source.used;
        current_index = (used - 1);
    }

    sequence operator +(const sequence& source1, const sequence& source2) {
        sequence result(source1.size() + source2.size());
        result += source1;
        result += source2;
        return result;
    }

    sequence::value_type sequence::operator[](size_type index) const {
        return data[index];
    }
}