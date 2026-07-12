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

    int getId() { return id; }
    string getName() { return name; }
    int getCreatorId() { return creatorId; }
    vector<int> getSongIds() { return songIds; }

    void setName(string newName) { name = newName; }

    void addSong(int songId) {
        songIds.push_back(songId);
    }

    void removeSong(int songId) {
        for (auto it = songIds.begin(); it != songIds.end(); ++it) {
            if (*it == songId) {
                songIds.erase(it);
                break;
            }
        }
    }
};

#endif // PLAYLIST_H
