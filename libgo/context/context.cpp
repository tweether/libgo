#include "context.h"

namespace co {
#if defined(LIBGO_SYS_Windows)
	void*& FiberScopedGuard::GetTlsContext()
	{
		static thread_local void* native = nullptr;
		return native;
	}
#else
	fcontext_t& Context::GetTlsContext()
	{
		static thread_local fcontext_t tls_context;
		return tls_context;
	}
#endif
}
