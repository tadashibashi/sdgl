#include <sdgl/ArgParser.h>
#include <sdgl/logging.h>

#include "FileWatcherCache.h"

/*
    1. Command line usage:
            program <path/to/json-config> <output-root>
    2. Read JSON file format:
    {
        "outdir": "path relative to application content folder" (default: "")
        "content": [
            {
                // Runs crunch on a folder
                "source": "relative/path/to/dir,relative/path/to/dir", // support converting .aseprite to png
                "dest": ""
                "processor": "sprite-atlas"
            },
            {
                // Copy a file or folder structure verbatim
                "source": "relative/path/to/dir/or/file/file",
                "dest": "" // if omitted, assumes the same as path in output directory
                "processor": "copy" // copy if newer
            }

        ]
    }


    If --process-only is not added, open the app interface (FUTURE)
    - File menu functionality
        1. New -> create project .json file and folder structure at location
        2. Open project
        3. Save
    - Build menu

    Application interface has a section for sprite editing.
        - in `/sprites` folder, individual sprite data, which consists:
            - one .aseprite or .ase file per sprite
                - each file name should be unique as folder paths do not get written into atlas data
                - converter should take frame speed into account
            - one .json file containing pivot point and collision mask per sprite
    Building tile maps
*/



int main(int argc, char *argv[])
{
    sdgl::ArgParser parser(argc, argv);

    sdgl::FileWatcherCache cache("test", "cache.txt", "items", [](const auto &path) {
        SDGL_LOG("File updated: {}", path);
    });

    const auto filesUpdated = cache.updateCache();

    SDGL_LOG("Files updated count: {}", filesUpdated);

    cache.printCache();

    cache.writeCache();

    return 0;
}
