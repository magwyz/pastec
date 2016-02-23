#ifndef ORBINDEXCOLLECTION
#define ORBINDEXCOLLECTION

#include <unordered_map>

#include <indexcollection.h>
#include <orb/orbindex.h>


class ORBIndexCollection : public IndexCollection
{
public:
    u_int32_t addIndex(const std::string indexName);
    u_int32_t removeIndex(const std::string indexName);

    u_int32_t get(const std::string indexName, Index ** const index);

private:
    u_int32_t checkIndexName(const std::string indexName);

    std::unordered_map<string, ORBIndex*> indexes;
};


#endif // ORBINDEXCOLLECTION

