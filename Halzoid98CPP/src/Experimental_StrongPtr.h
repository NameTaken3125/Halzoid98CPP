#pragma once

#include "ACU/basic_types.h"
#include "ACU_SharedPtrs.h"

SharedBlock* const g_emptySharedBlock = (SharedBlock*)0x14525BB58;
class ACUSharedPtr_Strong
{
public:
    // "Empty" shared ptr.
    ACUSharedPtr_Strong();
    ACUSharedPtr_Strong(uint64 managedObjectHandle);
    ACUSharedPtr_Strong(SharedPtrNew<ManagedObject>& sharedBlock);
    ACUSharedPtr_Strong(const ACUSharedPtr_Strong& rhs);
    ACUSharedPtr_Strong& operator=(const ACUSharedPtr_Strong& rhs);
    ~ACUSharedPtr_Strong();

public:
    template<class ManagedObjectSubcls>
    ManagedObjectSubcls* GetPtr() { return static_cast<ManagedObjectSubcls*>(m_SharedBlock.get().GetPtr()); }
    template<class ManagedObjectSubcls>
    SharedPtrNew<ManagedObjectSubcls>& GetSharedBlock() { return (SharedPtrNew<ManagedObjectSubcls>&)m_SharedBlock.get(); }
    void Reset();
private:
    // In the game, the pointer to a shared block is never a `nullptr`.
    // When a shared ptr is reset to "empty", it just points to `g_emptySharedBlock` (== 0x14525BB58).
    // It's an optimisation thing, I guess, to avoid nullptr-checking when increasing/decreasing the refcount.
    std::reference_wrapper<SharedBlock> m_SharedBlock;
};
assert_sizeof(ACUSharedPtr_Strong, 8);



inline void ACUSharedPtr_Strong::Reset()
{
    SharedBlock& sharedBlock = m_SharedBlock.get();
    m_SharedBlock = *g_emptySharedBlock;
    sharedBlock.DecrementStrongRefcount();
}
inline ACUSharedPtr_Strong::ACUSharedPtr_Strong(SharedPtrNew<ManagedObject>& sharedBlock)
    : m_SharedBlock(static_cast<SharedBlock&>(sharedBlock))
{
    sharedBlock.IncrementStrongRefcount();
}
inline ACUSharedPtr_Strong::ACUSharedPtr_Strong(const ACUSharedPtr_Strong& rhs)
    : m_SharedBlock(rhs.m_SharedBlock)
{
    m_SharedBlock.get().IncrementStrongRefcount();
}
inline ACUSharedPtr_Strong& ACUSharedPtr_Strong::operator=(const ACUSharedPtr_Strong& rhs)
{
    SharedBlock& prevSharedBlock = m_SharedBlock.get();
    m_SharedBlock = rhs.m_SharedBlock;
    m_SharedBlock.get().IncrementStrongRefcount();
    prevSharedBlock.DecrementStrongRefcount();
    return *this;
}
inline ACUSharedPtr_Strong::ACUSharedPtr_Strong(uint64 handle)
    : m_SharedBlock(FindOrMakeSharedBlockByHandleAndIncrementStrongRefcount(handle))
{}
inline ACUSharedPtr_Strong::ACUSharedPtr_Strong()
    : m_SharedBlock(*g_emptySharedBlock)
{}
inline ACUSharedPtr_Strong::~ACUSharedPtr_Strong()
{
    m_SharedBlock.get().DecrementStrongRefcount();
}
