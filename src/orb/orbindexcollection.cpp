
#include <orbindexcollection.h>

#include <messages.h>


u_int32_t ORBIndexCollection::checkIndexName(const std::string indexName)
{
    if (indexName.length() >= INDEX_NAME_MAX_LEN)
        return INDEX_NAME_TOO_LONG;

    return OK;
}


u_int32_t ORBIndexCollection::addIndex(const std::string indexName)
{
    u_int32_t ret = checkIndexName(indexName);
    if (ret != OK)
        return ret;

    if (indexes.find(indexName) != indexes.end())
        return INDEX_ALREADY_EXISTING;

    ORBIndex *p_index = new(std::nothrow) ORBIndex("", false);
    if (p_index == NULL)
        return ERROR_MEMORY;

    indexes[indexName] = p_index;

    return OK;
}


/**
 * @brief Remove an index from the collection.
 * @param indexName the index name
 * @return OK else an error
 */
u_int32_t ORBIndexCollection::removeIndex(const std::string indexName)
{
    u_int32_t ret = checkIndexName(indexName);
    if (ret != OK)
        return ret;

    std::unordered_map<string, ORBIndex*>::const_iterator indexIt
            = indexes.find(indexName);

    if (indexIt == indexes.end())
        return INDEX_UNKNOWN;

    // TODO: check that there are no ongoing operations...
    delete indexIt->second;
    indexes.erase(indexIt);

    return OK;
}


/**
 * @brief Get an index from the collection.
 * @param indexName the index name
 * @param index a pointer to return a pointer to the index
 * @return OK else an error
 */
u_int32_t ORBIndexCollection::get(const std::string indexName, Index ** const index)
{
    u_int32_t ret = checkIndexName(indexName);
    if (ret != OK)
        return ret;

    std::unordered_map<string, ORBIndex*>::const_iterator indexIt
            = indexes.find(indexName);

    cout << "indeName: " << indexName << endl;
    if (indexIt == indexes.end())
        return INDEX_UNKNOWN;

    *index = indexIt->second;

    return OK;
}
