#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <optional>

// Mocking fmt for basic string concatenation to avoid dependency in this simple test if possible,
// or just use standard streams.
// But the project uses fmt, so we might want to test fmt if we can link it.
// For now, let's stick to standard library to test std::filesystem::path behavior specifically.

void CheckPath(const std::string& label, const std::filesystem::path& p) {
    std::cout << "Checking " << label << "..." << std::endl;
    try {
        // This is the operation that usually fails on Windows with non-ANSI characters
        // if the system locale is not UTF-8.
        std::string s = p.string();
        std::cout << "  p.string() success: " << s << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  p.string() FAILED: " << e.what() << std::endl;
    }

    try {
        std::string u8 = reinterpret_cast<const char*>(p.u8string().c_str());
        std::cout << "  p.u8string() success: " << u8 << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  p.u8string() FAILED: " << e.what() << std::endl;
    }
}

int main() {
    // The path causing issues:
    // D:/CS/Mapper/Source/kz/xiaobai/hb_xiaobai2026/Model/樱花树/cherry_tree.mdl
    
    // We construct it using UTF-8 string literal (assuming this source file is saved as UTF-8)
    // On MSVC with /utf-8 or similar, standard string literals might work, but u8 guarantees it.
    // However, C++20 u8 literals are char8_t, which doesn't implicitly convert to std::string (char).
    const char8_t* rawPathU8 = u8"D:/CS/Mapper/Source/kz/xiaobai/hb_xiaobai2026/Model/樱花树/cherry_tree.mdl";
    std::string rawPath(reinterpret_cast<const char*>(rawPathU8));
    
    std::cout << "Original UTF-8 string size: " << rawPath.size() << std::endl;
    
    std::filesystem::path p = std::filesystem::u8path(rawPath);
    
    CheckPath("Full Path", p);
    
    // Test parent path
    std::filesystem::path parent = p.parent_path();
    CheckPath("Parent Path", parent);
    
    // Test stem
    std::filesystem::path stem = p.stem();
    CheckPath("Stem", stem);
    
    // Test iteration
    std::cout << "Iterating path components:" << std::endl;
    for (const auto& part : p) {
        CheckPath("Part", part);
    }
    
    return 0;
}
