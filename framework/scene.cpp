#include "scene.hpp"

void parse_sdf_file(const std::string& sdf_file_path, Scene& scene) {
    // Datei öffnen
    std::ifstream sdf_file(sdf_file_path);
    // falls nicht geöffnet → fehler
    if (!sdf_file.is_open()) {
        std::cerr << "Could not find or open: " << sdf_file_path << std::endl;
        return;
    }

    // string um datei zeile für zeile zu lesen
    std::string line_buffer;

    // getline reads an input-stream (here: our filestream) line by
    // line (i.e. until a newline character) and stores the read
    // results in a std::string object (our line buffer)
    while (std::getline(sdf_file, line_buffer)) {
        std::istringstream line_as_stream(line_buffer);
        std::string token;
        line_as_stream >> token;

        if ("define" == token) {
            // get the next token, can only be 'material' for now
            line_as_stream >> token;

            if ("material" == token) {
                std::shared_ptr<Material> parsed_material = std::make_shared<Material>();

                // parse the remaining expected parameters one by one and in order
                // into our struct (you need to dereference your pointer, of course)
                line_as_stream >> parsed_material->name_;

                // ambient
                line_as_stream >> parsed_material->ka.r;
                line_as_stream >> parsed_material->ka.g;
                line_as_stream >> parsed_material->ka.b;

                // diffuse
                line_as_stream >> parsed_material->kd.r;
                line_as_stream >> parsed_material->kd.g;
                line_as_stream >> parsed_material->kd.b;

                // specular
                line_as_stream >> parsed_material->ks.r;
                line_as_stream >> parsed_material->ks.g;
                line_as_stream >> parsed_material->ks.b;

                // Spekularreflexionsexponent
                line_as_stream >> parsed_material->m;

                // push material into scene
                scene.material_container.push_back(parsed_material);
            } else {
                std::cerr << "Unexpected keyword: " << token << std::endl;
            }
        } else {
            std::cerr << "Unexpected keyword: " << token << std::endl;
        }
    }
    // close our open file at the end
    sdf_file.close();
}