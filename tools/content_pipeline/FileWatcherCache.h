#pragma once
#include <sdgl/sdglib.h>

namespace sdgl {
    /// Contains functionality to track system files for updates, and reports them to a callback.
    /// It does not contain file watcher, but can easily be plugged in to such functionality.
    ///
    /// The generated cache file is a binary file in little-endian format, all integers are assumed to be unsigned
    /// unless otherwise noted:
    ///
    /// 4 bytes - "SF$" stands for Sdgl File Cache ($=cash lol), plus one ubyte for version number (1)
    ///
    /// uint32  - byte length of contents after these 8 bytes (write this in after finished composing file)
    ///
    /// ----- HEAD BLOCK ------------------------------------------------------------------------------------------
    ///
    /// 4 bytes - "head"
    ///
    /// uint32  - length of this block, will always be 32
    ///
    /// 4 bytes - 4 chars representing cache id
    ///
    /// uint32 - number of files represented in the cache
    ///
    /// 24 reserved bytes
    ///
    /// ----- DATA BLOCK ------------------------------------------------------------------------------------------
    ///
    /// 4 bytes - "data"
    ///
    /// uint32 - length of this block in bytes
    ///
    /// Repeated file data - the `head` block indicates how many number of file data should be present:
    ///
    ///     - Null-terminated c-string
    ///
    ///     - uint64 - milliseconds since epoch since last updated
    ///
    class FileWatcherCache
    {
    public:
        /// @param id 4-char identifier for this file cache, checks against the cache file just in case (we don't want
        ///           to populate this object with the wrong data), any string greater than 4 gets truncated down to 4,
        ///           any less, and it will check for a shorter string, length must be > 0
        /// @param cachePath path to the cache binary file
        /// @param folderPath path to the folder to check for file changes
        /// @param onFileUpdated callback that fires when `checkCache` is called, for each file that has been updated
        ///                      more recently than the timestamp in the cache
        /// @param exts file extensions to watch updates for (including prefixed '.'); an empty vector means that all
        ///             file types are selected. An empty string means files with no extensions.
        ///             Examples of valid extensions: ".png", ".xml", "", ".exe", etc.
        FileWatcherCache(string_view id, fs::path cachePath, fs::path folderPath, func<void(const fs::path &filepath)> onFileUpdated,
            vector<string> exts = {});

        /// Run a recursive check on each file that resides in the same tree as the cache file's parent folder.
        /// Updates the cache with new information, and fires the onFileUpdated callback for each updated file.
        /// @param outFilenames a vector to receive each updated file name
        /// @returns the number of updated
        uint updateCache(vector<string> *outFilenames = nullptr);

        /// Writes data to cache file
        /// @returns whether operation succeeded
        bool writeCache() const;

        void printCache() const;
    private:
        /// Populate the cache with data from a file
        /// @returns whether data was populated, will return false if no file exists at that location yet
        bool readCache();

        /// Contains timestamps for each file in the same tree as `m_cachePath`
        map<fs::path, uint64> m_cache;

        /// Path to the binary cache file
        fs::path m_cachePath;

        /// Path to the directory to check for changes
        fs::path m_folderPath;

        /// Cache identifier, checks the cache file to ensure it's working on the expected cache file
        string m_id;

        /// Callback that fires for each file has been updated more recently than the cached timestamp when
        /// `updateCache` is called
        func<void(const fs::path &filepath)> m_onFileUpdated;

        /// List of file extensions to look for, an entry with empty string means no extension is acceptable;
        /// an empty list means that all file types are acceptable
        vector<string> m_exts;
    };
}
