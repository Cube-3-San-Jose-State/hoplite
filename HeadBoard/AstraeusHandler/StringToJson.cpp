/**
 * JSON deserializer for incoming and outgoing pipes. Tries to convert values to double, then int, then strings.
 * Responsiblities: Deserialize/converts incoming and outgoing strings into JSON objects of type double, int, and then string.
*/

// include statements from C/C++ library and/or Astraeus
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

// Function to check if char is whitespace
bool isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

// Function to extract string in double quotes
std::string extractString(std::string& str, int& i) {
    i++; // Skip opening quote
    std::string value;
    while(i < str.length() && str[i] != '"') {
        value += str[i];
        i++;
    }
    i++; // Skip closing quote
    return value;
}

// Function to parse JSON key-value pair with type conversion
std::pair<std::string, std::any> parseKeyValuePair(std::string& str, int& i) {
    
    // Skip whitespace
    while(isWhitespace(str[i])) {
        i++;
    }

    // Extract key
    std::string key = extractString(str, i);

    // Error handling: check for colon
    if (str[i] != ':') {
        return {"", ""};
    }
    i++;

    // Skip whitespace
    while(isWhitespace(str[i])) {
        i++;
    }

    // Extract value
    std::string initialValue = extractString(str, i);
    std::any value;
    
    try {
        // Convert to double
        value = std::stod(initialValue);
    } catch (const std::invalid_argument&) {
        try {
        // If double conversion fails, try int
        value = std::stoi(initialValue);
        } catch (const std::invalid_argument&) {
        // If both conversion fail, keep as string
        value = initialValue;
        }
    }

    return {key,value};
}

// Function to parse string to JSON object with type conversion
std::unordered_map<std::string, std::any> stringToJson(std::string& str) {
    std::unordered_map<std::string, std::any> json;
    int i = 0;

    // Error handling: skip opening curly brace
    if (str[i] != '{') {
        return json;
    }
    i++;

    // Parse key-value pairs until closing curly brace
    while (i < str.length() && str[i] != '}') {
        
        std::pair<std::string, std::any> keyValue = parseKeyValuePair(str, i);
        
        if (!keyValue.first.empty()) {
            json[keyValue.first] = keyValue.second;
        }

        // Skip whitespace and comma
        while (isWhitespace(str[i])) {
            i++;
        }
        if (str[i] == ',') {
            i++;
        }
    }

    // Error handling: skip closing curly brace
    if (str[i] != '}') {
        return json;
    }
    i++;

    return json;
}

// Driver
int main(int argc, char *argv[])
{
    std::string json = "{\"process\":\"ASTRAEUS\",\"dir\":\"OUTGOING\",\"function\":\"REQ_TEL\",\"arguments\":[\"11111111\"]}";
    std::unordered_map<std::string,std::any> s = stringToJson(json);
     for (auto& keyValue : s) {
      std::cout << keyValue.first << ": " << keyValue.second << std::endl;
    }
}
