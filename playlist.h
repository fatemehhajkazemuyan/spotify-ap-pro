#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>

using namespace std;

class Playlist {
private:
    int id;
    string name;
    int creatorId;
    vector<int> songIds;

public:
    Playlist(int _id, string _name, int _creatorId) {
        id = _id;
        name = _name;
        creatorId = _creatorId;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    int getCreatorId() const { return creatorId; }
    vector<int> getSongIds() const { return songIds; }

    void setName(string newName) { name = newName; }

    void addSong(int songId) {
        songIds.push_back(songId);
    }

    void removeSong(int songId) {
        for (size_t i = 0; i < songIds.size(); i++) {
            if (songIds[i] == songId) {
                songIds.erase(songIds.begin() + i);
                break;
            }
        }
    }
};

#endif // PLAYLIST_H
