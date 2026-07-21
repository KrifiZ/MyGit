#include <string>
class ZLibWrapper {
public:
	void compressBlob(const std::string& hash, const std::string& path);
};