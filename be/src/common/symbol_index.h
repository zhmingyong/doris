// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
// This file is copied from
// https://github.com/ClickHouse/ClickHouse/blob/master/src/Common/SymbolIndex.h
// and modified by Doris

#pragma once

#if defined(__ELF__) && !defined(__FreeBSD__)

#include <assert.h>
#include <common/elf.h>
#include <common/multi_version.h>

#include <boost/noncopyable.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace doris {

/** Allow to quickly find symbol name from address.
  * Used as a replacement for "dladdr" function which is extremely slow.
  * It works better than "dladdr" because it also allows to search private symbols, that are not participated in shared linking.
  */
class SymbolIndex : private boost::noncopyable {
protected:
    SymbolIndex() { update(); }

public:
    static MultiVersion<SymbolIndex>::Version instance();
    static void reload();

    struct Symbol {
        const void* address_begin = nullptr;
        const void* address_end = nullptr;
        const char* name = nullptr;
    };

    struct Object {
        const void* address_begin = nullptr;
        const void* address_end = nullptr;
        std::string name;
        std::shared_ptr<Elf> elf;
    };

    /// Address in virtual memory should be passed. These addresses include offset where the object is loaded in memory.
    const Symbol* findSymbol(const void* address) const;
    const Object* findObject(const void* address) const;

    const std::vector<Symbol>& symbols() const { return data.symbols; }
    const std::vector<Object>& objects() const { return data.objects; }

    std::string_view getResource(std::string name) const {
        if (auto it = data.resources.find(name); it != data.resources.end()) {
            return it->second.data();
        }
        return {};
    }

    /// The BuildID that is generated by compiler.
    std::string getBuildID() const { return data.build_id; }
    std::string getBuildIDHex() const;

    struct ResourcesBlob {
        /// Symbol can be presented in multiple shared objects,
        /// base_address will be used to compare only symbols from the same SO.
        ElfW(Addr) base_address = 0;
        /// Just a human name of the SO.
        std::string_view object_name;
        /// Data blob.
        std::string_view start;
        std::string_view end;

        [[nodiscard]] std::string_view data() const {
            assert(end.data() >= start.data());
            return std::string_view {start.data(), static_cast<size_t>(end.data() - start.data())};
        }
    };
    using Resources = std::unordered_map<std::string_view /* symbol name */, ResourcesBlob>;

    struct Data {
        std::vector<Symbol> symbols;
        std::vector<Object> objects;
        std::string build_id;

        /// Resources (embedded binary data) are located by symbols in form of _binary_name_start and _binary_name_end.
        Resources resources;
    };

private:
    Data data;

    void update();
    static MultiVersion<SymbolIndex>& instanceImpl();
};

} // namespace doris

#endif