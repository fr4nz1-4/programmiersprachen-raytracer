#include <glm/glm.hpp>
#include <iostream>
//file stream for opening files
#include <fstream>
#include <string>
// stringstream for getting parts of the read string (tokens) and
// streaming them into our Material Variables
#include <sstream>
#include <vector>
#include "scene.hpp"
#include "material.hpp"

// we want to read sdf-filepaths as command line arguments
int main(int argc, char** argv)
{
    // the first command line argument (argv[0]) is always the program name
    // we need at least 2 arguments (name and the actual sdf path) to continue
    // in a sensible way
    if (argc < 2) {
        // if we have less than 2 arguments, we tell the user how we want
        // to call the application and exit the program
        std::cout << "Please call the program in the following way:" << std::endl;
        std::cout << argv[0] << " <sdf_file_path.sdf>" << std::endl;
        return -1;
    }

    // we parse the first "real" argument and save it in a string
    // this should contain our sdf-file-path
    std::string const sdf_file_path = argv[1];

    // we try to open the file using our string
    // implicitly trying to open the file stream
    std::ifstream sdf_file(sdf_file_path);

    // if we could not open the file (i.e. because we mistyped the path)
    // we tell the user and exit the program
    if (!sdf_file.is_open()) {
        std::cout << "Could not find or open: " << sdf_file_path << std::endl;
        return -1;
    }

    // create a vector of Material (caution, you should use shared pointers)
    std::vector<Material> mat_vec;

    // we need a string as line buffer to read the sdf file line by line
    std::string line_buffer;

    // debug info
    // uint32_t line_count = 0;

    // getline reads an input-stream (here: our filestream) line by
    // line (i.e. until a newline character) and stores the read
    // results in a std::string object (our line buffer)
    while (std::getline(sdf_file, line_buffer)) {
        // debug info
        // std::cout << line_count << ": " << line_buffer << std::endl;
        // ++line_count;

        // we construct an istringstream from our line buffer
        std::istringstream line_as_stream(line_buffer);

        // from here on forward we can use our stringstream object
        // to stream the next token (i.e. word until next whitespace
        // character) into compatible variables
        std::string token;
        // give us the first token (should be 'define')
        line_as_stream >> token;
        // if so, we continue
        if ("define" == token) {
            // get the next token, can only be 'material' for now
            line_as_stream >> token;
            // if so, we continue
            if ("material" == token) {

                // create a material (you should create a shared_ptr to Material)
                Material parsed_material;

                // parse the remaining expected parameters one by one and in order
                // into our struct (you need to dereference your pointer, of course)
                line_as_stream >> parsed_material.name_;

                // use a loop for convenience because we use glm::vec3s
                // the first 3 floats represent ambient light reflection
                // capabilities of the material (ka_red, ka_green, ka_blue)
                for (int ka_index = 0; ka_index < 3; ++ka_index) {
                    line_as_stream >> parsed_material.ka[ka_index];
                }

                // use a loop for convenience because we use glm::vec3s
                // the second 3 floats represent diffuse light reflection
                // capabilities of the material (kd_red, kd_green, kd_blue)
                for (int kd_index = 0; kd_index < 3; ++kd_index) {
                    line_as_stream >> parsed_material.ka[kd_index];
                }

                // use a loop for convenience because we use glm::vec3s
                // the second 3 floats represent specular light reflection
                // capabilities of the material (ks_red, ks_green, ks_blue)
                for (int ks_index = 0; ks_index < 3; ++ks_index) {
                    line_as_stream >> parsed_material.ks[ks_index];
                }

                // the last float represents the specular power of the material (i.e.
                // whether we create hard, small highlights, or large, soft highlights)
                line_as_stream >> parsed_material.m;

                // now we can push our material into our vector (you should of course use shared_ptrs)
                mat_vec.push_back(parsed_material);

                // just as a sanity check, print the material attributes
                std::cout << "Parsed material " <<
                          parsed_material.name_ << " "
                          << parsed_material.ka[0] << " " << parsed_material.ka[1] << " ..." << std::endl;
            }
            else {
                std::cout << "Unexpected keyword: " << token << std::endl;
            }
        }
        else {
            std::cout << "Unexpected keyword: " << token << std::endl;
        }

    }

    // close our open file at the end
    sdf_file.close();

    return 0;
}