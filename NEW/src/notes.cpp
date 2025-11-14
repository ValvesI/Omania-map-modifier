#include "notes.hpp"
#include "beatmap.hpp"

void Note::updateColumn() {
    const int totalWidth = 512;
    const int numColumns = this->map->keyCount;

    int columnWidth = totalWidth / numColumns;
    column = x / columnWidth;      // mapeia x → coluna
}
