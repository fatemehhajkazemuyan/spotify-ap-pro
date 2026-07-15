#ifndef ALBUM_REPOSITORY_H
#define ALBUM_REPOSITORY_H

#include "abstractrepository.h"
#include "album.h"
#include <vector>
#include <memory>

using namespace std;

class AlbumRepository : public AbstractRepository<Album> {
public:
    int save(shared_ptr<Album> entity) override;
    bool remove(int id) override;
    shared_ptr<Album> search(int id) const override;
    vector<shared_ptr<Album>> albums(int artistId) const;
};

#endif // ALBUM_REPOSITORY_H
