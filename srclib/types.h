#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

#pragma pack(push, 1)

typedef struct
{
  uint8 VbeSignature[4]; // 'VESA' VBE Signature
  uint16 VbeVersion;     // 0300h VBE Version
  uint32 OemStringPtr;   // VbeFarPtr to OEM String
  uint8 Capabilities[4]; // Capabilities of graphics controller
  uint32 VideoModePtr;   // VbeFarPtr to VideoModeList
  uint16 TotalMemory;    // Number of 64kb memory blocks
  // Added for VBE 2.0+
  uint16 OemSoftwareRev;    // VBE implementation Software revision
  uint32 OemVendorNamePtr;  // VbeFarPtr to Vendor Name String
  uint32 OemProductNamePtr; // VbeFarPtr to Product Name String
  uint32 OemProductRevPtr;  // VbeFarPtr to Product Revision String
  uint8 Reserved[222];      // Reserved for VBE implementation scratch
  // area
  uint8 OemData[256]; // Data Area for OEM Strings
} tVbeInfo;

typedef struct
{
  // Mandatory information for all VBE revisions
  uint16 ModeAttributes;   // mode attributes
  uint8 WinAAttributes;    // window A attributes
  uint8 WinBAttributes;    // window B attributes
  uint16 WinGranularity;   // window granularity
  uint16 WinSize;          // window size
  uint16 WinASegment;      // window A start segment
  uint16 WinBSegment;      // window B start segment
  uint32 WinFuncPtr;       // real mode pointer to window function
  uint16 BytesPerScanLine; // bytes per scan line
  // Mandatory information for VBE 1.2 and above
  uint16 XResolution;       // horizontal resolution in pixels or characters
  uint16 YResolution;       // vertical resolution in pixels or characters
  uint8 XCharSize;          // character cell width in pixels
  uint8 YCharSize;          // character cell height in pixels
  uint8 NumberOfPlanes;     // number of memory planes
  uint8 BitsPerPixel;       // bits per pixel
  uint8 NumberOfBanks;      // number of banks
  uint8 MemoryModel;        // memory model type
  uint8 BankSize;           // bank size in KB
  uint8 NumberOfImagePages; // number of images
  uint8 rsvd1;              // reserved for page function
  // Direct Color fields (required for direct/6 and YUV/7 memory models)
  uint8 RedMaskSize;         // size of direct color red mask in bits
  uint8 RedFieldPosition;    // bit position of lsb of red mask
  uint8 GreenMaskSize;       // size of direct color green mask in bits
  uint8 GreenFieldPosition;  // bit position of lsb of green mask
  uint8 BlueMaskSize;        // size of direct color blue mask in bits
  uint8 BlueFieldPosition;   // bit position of lsb of blue mask
  uint8 RsvdMaskSize;        // size of direct color reserved mask in bits
  uint8 RsvdFieldPosition;   // bit position of lsb of reserved mask
  uint8 DirectColorModeInfo; // direct color mode attributes
  // Mandatory information for VBE 2.0 and above
  uint32 PhysBasePtr; // physical address forflat memory frame buffer
  uint32 rsvd2;       // Reserved - always set to 0
  uint16 rsvd3;       // Reserved - always set to 0
  // Mandatory information for VBE 3.0 and above
  uint16 LinBytesPerScanLine;  // bytes per scan line for linear modes
  uint8 BnkNumberOfImagePages; // number of images for banked modes
  uint8 LinNumberOfImagePages; // number of images for linear modes
  uint8 LinRedMaskSize;        // size of direct color red mask (linear modes)
  uint8 LinRedFieldPosition;   // bit position of lsb of red mask (linear modes)
  uint8 LinGreenMaskSize;      // size of direct color green mask (linear modes)
  uint8 LinGreenFieldPosition; // bit position of lsb of green mask (linear modes)
  uint8 LinBlueMaskSize;       // size of direct color blue mask (linear modes)
  uint8 LinBlueFieldPosition;  // bit position of lsbof blue mask (linear modes)
  uint8 LinRsvdMaskSize;       // size of direct color reserved mask (linear modes)
  uint8 LinRsvdFieldPosition;  // bit position of lsb of reserved mask (linear modes)
  uint32 MaxPixelClock;        // maximum pixel clock (in Hz) for graphics mode
  uint8 rsvd4[189];            // remainder of ModeInfoBlock
} tVbeModeInfo;

#pragma pack(pop)

#endif