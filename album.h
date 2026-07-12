#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <vector>

using namespace std;

class Album {
private:
    int id;
    string name;
    int artistId;
    int releaseYear;
    vector<int> songIds;

public:
    Album(int _id, string _name, int _artistId, int _releaseYear) {
        id = _id;
        name = _name;
        artistId = _artistId;
        releaseYear = _releaseYear;
    }

    int getId() { return id; }
    string getName() { return name; }
    int getArtistId() { return artistId; }
    int getReleaseYear() { return releaseYear; }
    vector<int> getSongIds() { return songIds; }

    void setName(string newName) { name = newName; }

    void addSong(int songId) {
        songIds.push_back(songId);
    }
};

#endif // ALBUM_H
