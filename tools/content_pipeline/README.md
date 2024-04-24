# SDGL Content Pipeline

Tool for processing items for the SDGL engine

1. Interprets json file inside a content directory
2. Runs content processor to change files, cache files that have already been processed
3. Transfer files to a specified build location (probably at/near a target game binary path)

## Direction

Intermediate directory for building files
Moves built files to target output

Standard output folder structure

content/
    atlas.bin
    atlas.png

### Graphics

All graphics that can be expressed as Texture2D should be compacted into one atlas 
(which may contain multiple pages)
Crunch uses a size maximum of 4096x4096

These should be compiled 

Content types to support:

Recursively searched folder of .pngs 
BMFont -> Bitmap Font

### Todo:

- Arg parser: to read arguments from command line
- 