/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <xenia/kernel/modules/xboxkrnl/fs/devices/disc_image_entry.h>

#include <xenia/kernel/modules/xboxkrnl/fs/gdfx.h>
#include <xenia/kernel/modules/xboxkrnl/fs/devices/disc_image_file.h>


using namespace xe;
using namespace xe::kernel;
using namespace xe::kernel::xboxkrnl;
using namespace xe::kernel::xboxkrnl::fs;


namespace {


class DiscImageMemoryMapping : public MemoryMapping {
public:
  DiscImageMemoryMapping(uint8_t* address, size_t length, xe_mmap_ref mmap) :
      MemoryMapping(address, length) {
    mmap_ = xe_mmap_retain(mmap);
  }

  virtual ~DiscImageMemoryMapping() {
    xe_mmap_release(mmap_);
  }

private:
  xe_mmap_ref mmap_;
};


}


DiscImageEntry::DiscImageEntry(Type type, Device* device, const char* path,
                               xe_mmap_ref mmap, GDFXEntry* gdfx_entry) :
    gdfx_entry_(gdfx_entry),
    Entry(type, device, path) {
  mmap_ = xe_mmap_retain(mmap);
}

DiscImageEntry::~DiscImageEntry() {
  xe_mmap_release(mmap_);
}

MemoryMapping* DiscImageEntry::CreateMemoryMapping(
    xe_file_mode file_mode, const size_t offset, const size_t length) {
  if (file_mode & kXEFileModeWrite) {
    // Only allow reads.
    return NULL;
  }

  size_t real_offset = gdfx_entry_->offset + offset;
  size_t real_length = length ?
      MIN(length, gdfx_entry_->size) : gdfx_entry_->size;
  return new DiscImageMemoryMapping(
      xe_mmap_get_addr(mmap_) + real_offset,
      real_length,
      mmap_);
}

X_STATUS DiscImageEntry::Open(
    KernelState* kernel_state,
    /* mode etc */
    XFile** out_file) {
  //*out_file = new DiscImageFile...
  return X_STATUS_NOT_IMPLEMENTED;
}
/*    size_t real_offset = gdfx_entry_->offset + byte_offset;
    size_t real_length = MIN(buffer_length, gdfx_entry_->size - byte_offset);
    xe_copy_memory(
        buffer, buffer_length,
        xe_mmap_get_addr(mmap_) + real_offset, real_length);
    *out_bytes_read = real_length;
    return X_STATUS_SUCCESS;*/