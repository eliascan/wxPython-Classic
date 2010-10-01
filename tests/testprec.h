#include "wx/wxprec.h"
#include "wx/cppunit.h"

// Custom test macro that is only defined when wxUIActionSimulator is available
// this allows the tests that do not rely on it to run on platforms that don't
// support it.
//
// FIXME: And while OS X does support it, more or less, too many tests
//        currently fail under it so disable all interactive tests there. They
//        should, of course, be reenabled a.s.a.p.
#if wxUSE_UIACTIONSIMULATOR && !defined(__WXOSX__)
    #define WXUISIM_TEST(test) CPPUNIT_TEST(test)
#else
    #define WXUISIM_TEST(test) (void)0
#endif

// define wxHAVE_U_ESCAPE if the compiler supports \uxxxx character constants
#if (defined(__VISUALC__) && (__VISUALC__ >= 1300)) || \
    (defined(__GNUC__) && (__GNUC__ >= 3))
    #define wxHAVE_U_ESCAPE

    // and disable warning that using them results in with MSVC 8+
    #if wxCHECK_VISUALC_VERSION(8)
        // universal-character-name encountered in source
        #pragma warning(disable:4428)
    #endif
#endif

// thrown when assert fails in debug build
class TestAssertFailure
{
public:
    TestAssertFailure(const wxString& file,
                      int line,
                      const wxString& func,
                      const wxString& cond,
                      const wxString& msg)
        : m_file(file),
          m_line(line),
          m_func(func),
          m_cond(cond),
          m_msg(msg)
    {
    }

    const wxString m_file;
    const int m_line;
    const wxString m_func;
    const wxString m_cond;
    const wxString m_msg;

    wxDECLARE_NO_ASSIGN_CLASS(TestAssertFailure);
};

// macro to use for the functions which are supposed to fail an assertion
#if wxDEBUG_LEVEL
    // some old cppunit versions don't define CPPUNIT_ASSERT_THROW so roll our
    // own
    #define WX_ASSERT_FAILS_WITH_ASSERT(cond) \
        { \
            bool throwsAssert = false; \
            try { cond ; } \
            catch ( const TestAssertFailure& ) { throwsAssert = true; } \
            if ( !throwsAssert ) \
                CPPUNIT_FAIL("expected assertion not generated"); \
        }
#else
    // there are no assertions in this build so just check that it fails
    #define WX_ASSERT_FAILS_WITH_ASSERT(cond) CPPUNIT_ASSERT(!(cond))
#endif

// these functions can be used to hook into wxApp event processing and are
// currently used by the events propagation test
class WXDLLIMPEXP_FWD_BASE wxEvent;

typedef int (*FilterEventFunc)(wxEvent&);
typedef bool (*ProcessEventFunc)(wxEvent&);

extern void SetFilterEventFunc(FilterEventFunc func);
extern void SetProcessEventFunc(ProcessEventFunc func);

extern bool IsNetworkAvailable();

// helper class setting the locale to "C" for its lifetime
class CLocaleSetter
{
public:
    CLocaleSetter() : m_locOld(setlocale(LC_ALL, "C")) { }
    ~CLocaleSetter() { setlocale(LC_ALL, m_locOld); }

private:
    const char * const m_locOld;
    wxDECLARE_NO_COPY_CLASS(CLocaleSetter);
};

// Macro that can be used to register the test with the given name in both the
// global unnamed registry so that it is ran by default and a registry with the
// same name as this test to allow running just this test individually.
//
// Notice that the name shouldn't include the "TestCase" suffix, it's added
// automatically by this macro.
//
// Implementation note: CPPUNIT_TEST_SUITE_[NAMED_]REGISTRATION macros can't be
// used here because they both declare the variable with the same name (as the
// "unique" name they generate is based on the line number which is the same
// for both calls inside the macro), so we need to do it manually.
#define wxREGISTER_UNIT_TEST(name) \
    static CPPUNIT_NS::AutoRegisterSuite< name##TestCase > \
        CPPUNIT_MAKE_UNIQUE_NAME( autoRegisterRegistry__ ); \
    static CPPUNIT_NS::AutoRegisterSuite< name##TestCase > \
        CPPUNIT_MAKE_UNIQUE_NAME( autoRegisterNamedRegistry__ )(#name "TestCase")