#ifndef INDEXCOLLECTION
#define INDEXCOLLECTION

#include <sys/types.h>
#include <string>

#include <index.h>


#define INDEX_NAME_MAX_LEN 50

class Index;


class IndexCollection
{
public:
    virtual u_int32_t addIndex(const std::string indexName) = 0;
    virtual u_int32_t removeIndex(const std::string indexName) = 0;

    virtual u_int32_t get(const std::string indexName, Index ** const index) = 0;
};


#endif // INDEXCOLLECTION

