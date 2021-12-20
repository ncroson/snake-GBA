
//{{BLOCK(head)

//======================================================================
//
//	head, 8x8@4, 
//	+ palette 256 entries, not compressed
//	+ 1 tiles not compressed
//	Total size: 512 + 32 = 544
//
//	Time-stamp: 2018-01-18, 20:48:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HEAD_H
#define GRIT_HEAD_H

#define headTilesLen 32
extern const unsigned short headTiles[16];

#define headPalLen 512
extern const unsigned short headPal[256];

#endif // GRIT_HEAD_H

//}}BLOCK(head)
