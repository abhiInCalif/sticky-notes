
#ifndef _SPACETOUCH_H
#define _SPACETOUCH_H

#include <cstdlib>

// Define Spacetouch export macros
#if defined(_MSC_VER) // Visual C++

#if SPACETOUCH_API_INTERNAL
#define SPACETOUCH_EXPORT
#elif SPACETOUCH_API_IMPLEMENTATION
#define SPACETOUCH_EXPORT __declspec(dllexport)
#else
#define SPACETOUCH_EXPORT __declspec(dllimport)
#endif

#define SPACETOUCH_EXPORT_CLASS SPACETOUCH_EXPORT
//#define SPACETOUCH_EXPORT_PLUGIN __declspec(dllexport)
/*
#elif !defined(SWIG)
#define SPACETOUCH_EXPORT __attribute__((visibility("default")))
#define SPACETOUCH_EXPORT_CLASS __attribute__((visibility("default")))
//#define SPACETOUCH_EXPORT_PLUGIN __attribute__((visibility("default")))
*/
#else
#define SPACETOUCH_EXPORT
#define SPACETOUCH_EXPORT_CLASS
//#define SPACETOUCH_EXPORT_PLUGIN
#endif

namespace Spacetouch {

	enum SPACETOUCH_EXPORT_CLASS ADCDataType {
		RAW = 0, 
		FILTERED = 1, 
		NORMALIZED_FILTERED = 2
	} ;
	enum SPACETOUCH_EXPORT_CLASS GestureState {
		STATE_POSSIBLE = 0, 
		STATE_PRESSDOWN_BEGAN = 1, 
		STATE_INPRESSDOWN = 2, 
		STATE_BEGAN = 3, 
		STATE_ONGOING = 4, 
		STATE_ENDED = 5, 
		STATE_FAILED = 6} ;

	//for internal use only
	class SPACETOUCH_EXPORT_CLASS Interface {
	private:
	    void * _implementation;
	    bool _ownMem;
	protected:
	    bool ownMem();
	    void releaseMem();
	    Interface(void *, bool);
	    Interface(const Interface & rhs);
	    Interface& operator=(const Interface & rhs);
	    virtual ~Interface(); 
	public:
	    void * getImpl();
	};

	class SPACETOUCH_EXPORT_CLASS Parser : public Interface{
	public:
	    Parser(const char *ininame);
	    ~Parser();

	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get number of sections in a dictionary
	      @return   int Number of sections found in dictionary

	      This function returns the number of sections found in a dictionary.
	      The test to recognize sections is done on the string stored in the
	      dictionary: a section name is given as "section" whereas a key is
	      stored as "section:key", thus the test looks for entries that do not
	      contain a colon.

	      This clearly fails in the case a section name contains a colon, but
	      this should simply be avoided.

	      This function returns -1 in case of error.
	     */
	    /*--------------------------------------------------------------------------*/
	    int getnsec();
	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get name for section n in a dictionary.
	      @param    n   Section number (from 0 to nsec-1).
	      @return   Pointer to char string

	      This function locates the n-th section in a dictionary and returns
	      its name as a pointer to a string statically allocated inside the
	      dictionary. Do not free or modify the returned string!

	      This function returns NULL in case of error.
	     */
	    /*--------------------------------------------------------------------------*/
	    const char * getsecname(int n);
	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Save dictionary to a loadable ini file
	      @param    f   Opened file pointer to dump to
	      @return   void

	      This function dumps a given dictionary into a loadable ini file.
	      It is Ok to specify @c stderr or @c stdout as output files.
	     */
	    /*--------------------------------------------------------------------------*/
	    void dump_ini(const char * f);
	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Save dictionary section to a loadable ini file
	      @param    s   Section name of dictionary to dump
	      @param    f   Opened file pointer to dump to
	      @return   void

	      This function dumps a given section of a given dictionary into a loadable ini
	      file.  It is Ok to specify @c stderr or @c stdout as output files.
	     */
	    /*--------------------------------------------------------------------------*/
	    void dumpsection_ini(const char * s, const char * f);
	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Dump dictionary to an opened file pointer.
	      @param    f   Opened file pointer to dump to.
	      @return   void

	      This function prints out the contents of a dictionary, one element by
	      line, onto the provided file pointer. It is OK to specify @c stderr
	      or @c stdout as output files. This function is meant for debugging
	      purposes mostly.
	     */
	    /*--------------------------------------------------------------------------*/
	    void dump(const char * f);
	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get the number of keys in a section of a dictionary.
	      @param    s   Section name of dictionary to examine
	      @return   Number of keys in section
	     */
	    /*--------------------------------------------------------------------------*/
	    int getsecnkeys(const char * s);
	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get the string associated to a key
	      @param    key     Key string to look for
	      @param    def     Default value to return if key not found.
	      @return   pointer to statically allocated character string

	      This function queries a dictionary for a key. A key as read from an
	      ini file is given as "section:key". If the key cannot be found,
	      the pointer passed as 'def' is returned.
	      The returned char pointer is pointing to a string allocated in
	      the dictionary, do not free or modify it.
	     */
	    /*--------------------------------------------------------------------------*/
	    const char * getstring(const char * key, const char * def);

	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get the string associated to a key, convert to an int
	      @param    key Key string to look for
	      @param    notfound Value to return in case of error
	      @return   integer

	      This function queries a dictionary for a key. A key as read from an
	      ini file is given as "section:key". If the key cannot be found,
	      the notfound value is returned.

	      Supported values for integers include the usual C notation
	      so decimal, octal (starting with 0) and hexadecimal (starting with 0x)
	      are supported. Examples:

	      - "42"      ->  42
	      - "042"     ->  34 (octal -> decimal)
	      - "0x42"    ->  66 (hexa  -> decimal)

	      Warning: the conversion may overflow in various ways. Conversion is
	      totally outsourced to strtol(), see the associated man page for overflow
	      handling.

	      Credits: Thanks to A. Becker for suggesting strtol()
	     */
	    /*--------------------------------------------------------------------------*/
	    int getint(const char * key, int notfound);

	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get the string associated to a key, convert to a double
	      @param    key Key string to look for
	      @param    notfound Value to return in case of error
	      @return   double

	      This function queries a dictionary for a key. A key as read from an
	      ini file is given as "section:key". If the key cannot be found,
	      the notfound value is returned.
	     */
	    /*--------------------------------------------------------------------------*/
	    double getdouble(const char * key, double notfound);

	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Get the string associated to a key, convert to a boolean
	      @param    key Key string to look for
	      @param    notfound Value to return in case of error
	      @return   integer

	      This function queries a dictionary for a key. A key as read from an
	      ini file is given as "section:key". If the key cannot be found,
	      the notfound value is returned.

	      A true boolean is found if one of the following is matched:

	      - A string starting with 'y'
	      - A string starting with 'Y'
	      - A string starting with 't'
	      - A string starting with 'T'
	      - A string starting with '1'

	      A false boolean is found if one of the following is matched:

	      - A string starting with 'n'
	      - A string starting with 'N'
	      - A string starting with 'f'
	      - A string starting with 'F'
	      - A string starting with '0'

	      The notfound value returned if no boolean is identified, does not
	      necessarily have to be 0 or 1.
	     */
	    /*--------------------------------------------------------------------------*/
	    int getboolean(const char * key, int notfound);


	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Set an entry in a dictionary.
	      @param    entry   Entry to modify (entry name)
	      @param    val     New value to associate to the entry.
	      @return   int     0 if Ok, -1 otherwise.

	      If the given entry can be found in the dictionary, it is modified to
	      contain the provided value. If it cannot be found, the entry is created.
	      It is Ok to set val to NULL.
	     */
	    /*--------------------------------------------------------------------------*/
	    int set(const char * entry, const char * val);


	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Delete an entry in a dictionary
	      @param    entry   Entry to delete (entry name)
	      @return   void

	      If the given entry can be found, it is deleted from the dictionary.
	     */
	    /*--------------------------------------------------------------------------*/
	    void unset(const char * entry);

	    /*-------------------------------------------------------------------------*/
	    /**
	      @brief    Finds out if a given entry exists in a dictionary
	      @param    entry   Name of the entry to look for
	      @return   integer 1 if entry exists, 0 otherwise

	      Finds out if a given entry exists in the dictionary. Since sections
	      are stored as keys with NULL associated values, this is the only way
	      of querying for the presence of sections in a dictionary.
	     */
	    /*--------------------------------------------------------------------------*/
	    int find_entry(const char * entry) ;
	};

	class Vector : public Interface {
	public:
	    SPACETOUCH_EXPORT Vector(int);
	    Vector(void *);
	    SPACETOUCH_EXPORT ~Vector();
	    
	    SPACETOUCH_EXPORT int size();
	    SPACETOUCH_EXPORT double get(int);
	    SPACETOUCH_EXPORT void rewrite(int, double);
	    SPACETOUCH_EXPORT void append(double);
	    SPACETOUCH_EXPORT double max();
	    SPACETOUCH_EXPORT double min();
	    SPACETOUCH_EXPORT double sum();
	    SPACETOUCH_EXPORT double mean();
	};

	class Sensor : public Interface{
	public:
	    SPACETOUCH_EXPORT Sensor(int ID, int capacity, Parser *);
	    Sensor(void *);
	    SPACETOUCH_EXPORT ~Sensor();

	    SPACETOUCH_EXPORT int ID();
	    SPACETOUCH_EXPORT bool isNoisy();
	    SPACETOUCH_EXPORT double getADC(ADCDataType);
	    SPACETOUCH_EXPORT int getTrend();
	    SPACETOUCH_EXPORT double getDeltaADC();
	};

	class Pipeline : public Interface{
	public:
	    SPACETOUCH_EXPORT Pipeline(Parser *);
	    Pipeline(void *);
	    SPACETOUCH_EXPORT ~Pipeline();

	    SPACETOUCH_EXPORT int numberOfChannels();
	    SPACETOUCH_EXPORT int capacity();
	    
	    SPACETOUCH_EXPORT Sensor getSensor(int);
	    SPACETOUCH_EXPORT double *getSensorADC(ADCDataType);

	    SPACETOUCH_EXPORT double *getDeltaXYZ();
	    SPACETOUCH_EXPORT double *getAbsoluteXYZ();
	    SPACETOUCH_EXPORT int *getTrend();
	    SPACETOUCH_EXPORT double *getDeltaADC();
	};

	class Gesturerecognizer : public Interface{
	public:
	    SPACETOUCH_EXPORT Gesturerecognizer(Pipeline *);
	    Gesturerecognizer(void *, bool);
	    SPACETOUCH_EXPORT virtual ~Gesturerecognizer();
	    SPACETOUCH_EXPORT Pipeline pipeline();
	    SPACETOUCH_EXPORT bool isEnabled();
	    SPACETOUCH_EXPORT GestureState state();
	    SPACETOUCH_EXPORT void setEnabled(bool);
	    SPACETOUCH_EXPORT virtual void reset();

	    SPACETOUCH_EXPORT virtual void transition() = 0;

	    SPACETOUCH_EXPORT static bool isMovingForward(int *);
	    SPACETOUCH_EXPORT static bool isMovingBackward(int *);
	    SPACETOUCH_EXPORT static bool isStill(int *);

	    SPACETOUCH_EXPORT static bool isMovingTowardsLoose(int, int *);
	    SPACETOUCH_EXPORT static bool isMovingTowardsStrict(int, int *);

	protected:
	    SPACETOUCH_EXPORT virtual void onStatePressdownBegan();
	    SPACETOUCH_EXPORT virtual void onStateInPressdown();
	    SPACETOUCH_EXPORT virtual void onStateBegan();
	    SPACETOUCH_EXPORT virtual void onStateOnGoing();
	    SPACETOUCH_EXPORT virtual void onStateEnded();
	    SPACETOUCH_EXPORT virtual void onStateFailed();

	};

	class Gesturelist : public Interface{
	public:
		SPACETOUCH_EXPORT Gesturelist();
		Gesturelist(void *);
		SPACETOUCH_EXPORT ~Gesturelist();

		SPACETOUCH_EXPORT void add(Gesturerecognizer *);
		SPACETOUCH_EXPORT void remove(Gesturerecognizer *);
		SPACETOUCH_EXPORT bool has(Gesturerecognizer *);
		SPACETOUCH_EXPORT void copy(Gesturelist *);
		SPACETOUCH_EXPORT void iterBegin();
		SPACETOUCH_EXPORT Gesturerecognizer * iterGet();
		SPACETOUCH_EXPORT bool isAllInStatePossible();
		SPACETOUCH_EXPORT void transitionAll();
	};

	class Swiperecognizer : public Gesturerecognizer{
	public:
	    SPACETOUCH_EXPORT Swiperecognizer(Pipeline *, int, Parser *);
	    Swiperecognizer(void *);
	    SPACETOUCH_EXPORT ~Swiperecognizer();

	    SPACETOUCH_EXPORT void reset();
	    SPACETOUCH_EXPORT void transition();

	    SPACETOUCH_EXPORT int direction();
	    SPACETOUCH_EXPORT void setSwipeSpeedLimit(double);
	};

	class Taprecognizer : public Gesturerecognizer{
	public:
	    SPACETOUCH_EXPORT Taprecognizer(Pipeline *, Parser *);
	    Taprecognizer(void *);
	    SPACETOUCH_EXPORT ~Taprecognizer();

	    SPACETOUCH_EXPORT void reset();
	    SPACETOUCH_EXPORT void transition();

	    SPACETOUCH_EXPORT void setPressdownSpeedLimit(double); 
	    SPACETOUCH_EXPORT void setPressupSpeedLimit(double);
	    SPACETOUCH_EXPORT void setPressdownDistLimit(double);
	    SPACETOUCH_EXPORT void setPressupDistLimit(double);
	};

	class SPACETOUCH_EXPORT_CLASS Proximityindicator : public Gesturerecognizer{
	private:
		double bound0, bound1, bound2;
	public:
	    Proximityindicator(Pipeline *, Parser *);
	    virtual ~Proximityindicator();

	    void transition();

	    virtual void withInBound2(); 
	    virtual void withInBound12();
	    virtual void withInBound01();
	    void adjustBounds(double);
	};

	class Longpressrecognizer : public Gesturerecognizer{
	public:
	    SPACETOUCH_EXPORT Longpressrecognizer(Pipeline *, Parser *);
	    Longpressrecognizer(void *);
	    SPACETOUCH_EXPORT ~Longpressrecognizer();

	    SPACETOUCH_EXPORT void reset();
	    SPACETOUCH_EXPORT void transition();

	    SPACETOUCH_EXPORT void setPressdownTimeLimit(double);
	    SPACETOUCH_EXPORT void setPressdownSpeedLimit(double); 
	    SPACETOUCH_EXPORT void setPressupSpeedLimit(double);
	    SPACETOUCH_EXPORT void setPressdownDistLimit(double);
	    SPACETOUCH_EXPORT void setPressupDistLimit(double);
	};

	//forward declaration
	class Listener;

	class Controller : public Interface{
	protected:
	    //listening to device actions
	    Listener *listener;
	public:
	    SPACETOUCH_EXPORT Controller(Listener*, Parser *);
	    SPACETOUCH_EXPORT ~Controller();

	    SPACETOUCH_EXPORT Gesturelist gesturelist();
	    SPACETOUCH_EXPORT Pipeline pipeline();
	    SPACETOUCH_EXPORT bool isGestureInstalled(Gesturerecognizer *);
	    SPACETOUCH_EXPORT void installGesture(Gesturerecognizer *);
	    SPACETOUCH_EXPORT void removeGesture(Gesturerecognizer *);
	    SPACETOUCH_EXPORT void installGesturelist(Gesturelist *);
	    SPACETOUCH_EXPORT void setAutoUpdateBaseline(bool);

	    SPACETOUCH_EXPORT bool openDevice();
	    SPACETOUCH_EXPORT void setupDevice();
	    SPACETOUCH_EXPORT void readDevice();
	    SPACETOUCH_EXPORT void closeDevice();

	    SPACETOUCH_EXPORT void run();
	};

	class SPACETOUCH_EXPORT_CLASS Listener{
	public:
	    virtual ~Listener();
	    virtual void onDidOpenDevice(Controller *);
	    virtual void onDidSetupDevice(Controller *);
	    virtual void onDidCloseDevice(Controller *);
	    virtual void onFrame(Controller *);
	    virtual void onDeviceOpenFailed(Controller *);
	};
}
#endif/*_SPACETOUCH_H*/