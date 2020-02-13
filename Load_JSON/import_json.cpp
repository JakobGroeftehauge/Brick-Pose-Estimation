//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Annotation data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

namespace quicktype {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    struct Flags {
    };

    struct Shape {
        std::string label;
        nlohmann::json line_color;
        nlohmann::json fill_color;
        std::vector<std::vector<double>> points;
        std::string shape_type;
        Flags flags;
    };

    struct Annotation {
        std::string version;
        Flags flags;
        std::vector<Shape> shapes;
        std::vector<int64_t> line_color;
        std::vector<int64_t> fill_color;
        std::string image_path;
        std::string image_data;
        int64_t image_height;
        int64_t image_width;
    };
}

namespace nlohmann {
    void from_json(const json & j, quicktype::Flags & x);
    void to_json(json & j, const quicktype::Flags & x);

    void from_json(const json & j, quicktype::Shape & x);
    void to_json(json & j, const quicktype::Shape & x);

    void from_json(const json & j, quicktype::Annotation & x);
    void to_json(json & j, const quicktype::Annotation & x);

    inline void from_json(const json & j, quicktype::Flags& x) {
    }

    inline void to_json(json & j, const quicktype::Flags & x) {
        j = json::object();
    }

    inline void from_json(const json & j, quicktype::Shape& x) {
        x.label = j.at("label").get<std::string>();
        x.line_color = quicktype::get_untyped(j, "line_color");
        x.fill_color = quicktype::get_untyped(j, "fill_color");
        x.points = j.at("points").get<std::vector<std::vector<double>>>();
        x.shape_type = j.at("shape_type").get<std::string>();
        x.flags = j.at("flags").get<quicktype::Flags>();
    }

    inline void to_json(json & j, const quicktype::Shape & x) {
        j = json::object();
        j["label"] = x.label;
        j["line_color"] = x.line_color;
        j["fill_color"] = x.fill_color;
        j["points"] = x.points;
        j["shape_type"] = x.shape_type;
        j["flags"] = x.flags;
    }

    inline void from_json(const json & j, quicktype::Annotation& x) {
        x.version = j.at("version").get<std::string>();
        x.flags = j.at("flags").get<quicktype::Flags>();
        x.shapes = j.at("shapes").get<std::vector<quicktype::Shape>>();
        x.line_color = j.at("lineColor").get<std::vector<int64_t>>();
        x.fill_color = j.at("fillColor").get<std::vector<int64_t>>();
        x.image_path = j.at("imagePath").get<std::string>();
        x.image_data = j.at("imageData").get<std::string>();
        x.image_height = j.at("imageHeight").get<int64_t>();
        x.image_width = j.at("imageWidth").get<int64_t>();
    }

    inline void to_json(json & j, const quicktype::Annotation & x) {
        j = json::object();
        j["version"] = x.version;
        j["flags"] = x.flags;
        j["shapes"] = x.shapes;
        j["lineColor"] = x.line_color;
        j["fillColor"] = x.fill_color;
        j["imagePath"] = x.image_path;
        j["imageData"] = x.image_data;
        j["imageHeight"] = x.image_height;
        j["imageWidth"] = x.image_width;
    }
}
