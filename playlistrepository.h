#ifndef PLAYLIST_REPOSITORY_H
#define PLAYLIST_REPOSITORY_H

#include "abstractrepository.h"
#include "playlist.h"
#include <vector>
#include <memory>

using namespace std;

class PlaylistRepository : public AbstractRepository<Playlist> {
public:
    int save(shared_ptr<Playlist> entity) override;
    bool remove(int id) override;
    shared_ptr<Playlist> search(int id) const override;
    void insertSong(int playlistId, int songId);
    void removeSong(int playlistId, int songId);
    vector<shared_ptr<Playlist>> playlists(int listenerId) const;
};

#endif // PLAYLIST_REPOSITORY_H
