#pragma once

#include "basic_types.h"
#include <intrin.h>

class ManagedObject;

class SharedBlock
{
public:
    // @members
    ManagedObject* manObj; //0x0000
    volatile long weakRefCount; //0x0008
    volatile long strongRefCountAndFlags; //0x000C
    uint64 handle; //0x0010

    // @helper_functions
    ManagedObject* GetPtr() { return (ManagedObject*)((uint64)manObj & ((int64)strongRefCountAndFlags << 32 >> 63)); } // see at 0x14082E776
    void IncrementWeakRefcount() { _InterlockedIncrement((volatile long*)&weakRefCount); }
    void IncrementStrongRefcount() { _InterlockedIncrement((volatile long*)&strongRefCountAndFlags); }
    void DecrementWeakRefcount();
    void DecrementStrongRefcount();
};
assert_offsetof(SharedBlock, weakRefCount, 8);
assert_offsetof(SharedBlock, strongRefCountAndFlags, 0xC);
assert_sizeof(SharedBlock, 0x18);
void FreeSharedBlock_impl(SharedBlock& sharedBlockForManagedObject);
void MarkManagedObjectForFreeingAfterZeroStrongRefcount_impl(SharedBlock& sharedBlock);
inline void SharedBlock::DecrementWeakRefcount()
{
    if (!_InterlockedDecrement(&weakRefCount))
    {
        // Refcount (weak refcount maybe?) reached 0.
        if (!(strongRefCountAndFlags & 0xFFFFFF))
        {
            FreeSharedBlock_impl(*this);
        }
    }
}
inline void SharedBlock::DecrementStrongRefcount()
{
    if (!(_InterlockedDecrement(&strongRefCountAndFlags) & 0xFFFFFF))
    {
        MarkManagedObjectForFreeingAfterZeroStrongRefcount_impl(*this);
    }
}
template<class ManagedObjectSubcls>
class SharedPtrNew : public SharedBlock
{
public:

    // @helper_functions
    ManagedObjectSubcls* GetPtr() { return static_cast<ManagedObjectSubcls*>(SharedBlock::GetPtr()); }
    void IncrementWeakRefcount() { SharedBlock::IncrementWeakRefcount(); }
    void IncrementStrongRefcount() { SharedBlock::IncrementStrongRefcount(); }
    void DecrementWeakRefcount() { SharedBlock::DecrementWeakRefcount(); }
    void DecrementStrongRefcount() { SharedBlock::DecrementStrongRefcount(); }
};
assert_sizeof(SharedPtrNew<ManagedObject>, sizeof(SharedBlock));
