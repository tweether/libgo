#pragma once
#include "../../common/config.h"
#include "../../common/error.h"
#include <WinSock2.h>
#include <Windows.h>
#include "../fcontext.h"
#include "../../common/dllExporter.h"

namespace co {

    struct FiberScopedGuard
    {
        FiberScopedGuard::FiberScopedGuard()
        {
            GetTlsContext() = ConvertThreadToFiber(nullptr);
        }
        FiberScopedGuard::~FiberScopedGuard()
        {
            ConvertFiberToThread();
            GetTlsContext() = nullptr;
        }
        DLL_CLASS_DECL static void*& GetTlsContext();
    };

    class Context
    {
    public:
        Context(fn_t fn, intptr_t vp, std::size_t stackSize)
        {
            DebugPrint(dbg_task, "valloc stack. size=%lu", (unsigned long)stackSize);

            SIZE_T commit_size = 4 * 1024;
            ctx_ = CreateFiberEx(commit_size,
                std::max<std::size_t>(stackSize, commit_size), FIBER_FLAG_FLOAT_SWITCH,
                (LPFIBER_START_ROUTINE)fn, (LPVOID)vp);
            if (!ctx_) {
                ThrowError(eCoErrorCode::ec_makecontext_failed);
                return;
            }
        }
        ~Context()
        {
            DeleteFiber(ctx_);
        }

        ALWAYS_INLINE void SwapIn()
        {
            SwitchToFiber(ctx_);
        }

        ALWAYS_INLINE void SwapOut()
        {
            SwitchToFiber(FiberScopedGuard::GetTlsContext());
        }

    private:
        void* ctx_;
    };

} // namespace co
