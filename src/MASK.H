// See LICENSE for details

//XMASKIM.H: structures used for storing and manipulating masked images

//Describes one alignment of a mask-image pair

typedef struct
{
   int16_t ImageWidth; //image width in addresses in display memory
                       //(also mask width in bytes)
   uint16_t ImagePtr;  //offset of image bitmap in display mem
   uint8_t *MaskPtr;   //pointer to mask bitmap
} AlignedMaskedImage;

//Describes all four alignments of a mask-image pair

typedef struct
{
   AlignedMaskedImage *Alignments[4]; //ptrs to AlignedMaskedImage
                                      //structs for four possible destination
                                      //image alignments
} MaskedImage;
