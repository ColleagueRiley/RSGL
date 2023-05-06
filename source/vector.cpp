#ifndef RSGL_H
#include "../RSGL.hpp"
#endif

#include <stdlib.h>
#include <string.h>

namespace RSGL {
        template<typename type>
        void vector<type>::push_back(type data) { 
            srcSize++;

            src = (type*)realloc(src, sizeof(type) * size());

            src[size() - 1] = data;
        }

        template<typename type>
        void vector<type>::push_back(vector<type> data) {
            for (int i = 0; i < data.size(); i++)
                push_back(data[i]);
        }

        template<typename type>
        void vector<type>::insert(int index, type data) {
            if (index > size())
                push_back(data);
            else
                src[index] = data;
        }

        template<typename type>
        void vector<type>::insert(int begin, vector<type> data) {
            for (int i = begin; i < data.size(); i++) {
                if (i < size())
                    src[i] = data[i];
                else
                    push_back(data[i]);
            }
        }

        template<typename type>
        void vector<type>::erase(int index) {
            index++;

            type* ahead;
            type* behind;

            int j = 0;
            for (int i = 0; i < size(); i++, j++) {
                if (i == index)
                    j--;

                src[j] = src[i];
            }

            srcSize--;

            src = (type*) realloc(src, sizeof(type) * size());
        }


        template<typename type>
        vector<type>::vector(type* data, size_t size) { 
            src = (type*)malloc(size * sizeof(type));

            srcSize = size;

            memcpy(src, data, this->size() * sizeof(type));
        }

        template<typename type>
        vector<type>::vector(std::initializer_list<type> list) {
            src = (type*)malloc(list.size() * sizeof(type));

            srcSize = list.size();

            for (type i : list)
                push_back(i);
        }

        template<typename type>
        vector<type>::vector(const char* str) {
            if (sizeof(type) == sizeof(char)) {
                size_t size;
                for (size = 0; str[size]; size++){ }

                vector((char*)str, size);
            }  
        }

        template<typename type>
        bool vector<type>::operator==(vector<type> vector2){
            if (vector2.size() != size())
                return false;

            return !memcmp(src, vector2.src, size() * sizeof(type));
        }
}