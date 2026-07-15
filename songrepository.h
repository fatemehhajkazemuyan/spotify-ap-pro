#ifndef SONG_REPOSITORY_H
#define SONG_REPOSITORY_H

#include "abstractrepository.h"
#include "song.h"
#include "playlistrepository.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class SongRepository : public AbstractRepository<Song> {
public:
    int save(shared_ptr<Song> entity) override;
    bool remove(int id) override;
    shared_ptr<Song> search(int id) const override;


    vector<shared_ptr<Song>> singleSongs(int artistId) const;
    vector<shared_ptr<Song>> getByAlbum(int albumId) const;
    vector<shared_ptr<Song>> getByArtist(int artistId) const;
    vector<shared_ptr<Song>> getByPlaylist(int playlistId, shared_ptr<PlaylistRepository> playlistRepo) const;
    vector<shared_ptr<Song>> getByLikedSongs(int listenerId, shared_ptr<PlaylistRepository> playlistRepo) const;
};

#endif // SONG_REPOSITORY_H
