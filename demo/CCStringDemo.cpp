#include "../src/CCString.h"
#include "../src/CCDecimal.h"
#include <iomanip>
#include <iostream>
#include <bitset>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

/**
 * Demonstrates the different types CCString can accept as inputs using its constructors;
 * for others, conversion operators are neccessary.
 */
void typesDemo()
{
    /**
     * Supported types:
     * std::string
     * const char*
     * char
     * 
     * bool
     * 
     * (u)int_(8,16,32,64)
     * float
     * (long) double
     * CCDecimal
     */

    // Variables of several example types:
    int64_t negOne = -1;
    double aTenth = 0.1;
    CCDecimal accurateTenth = "0.1";
    bool trueValue = true;
    bool falseValue = false;

    CCString builtInTypes;
    builtInTypes << "Here, we have...:\n "
                 << "  an intege:   " << negOne << "\n"
                 << "  a double:    " << aTenth << "\n"
                 << "  a CCDecimal: " << accurateTenth << "\n"
                 << "  and the boolean values, " << trueValue << " and " << falseValue << "\n";

    struct demoRational
    {
        int a, b;
        /**
         * other types need a conversion operator for automatic conversion
         */
        operator CCString() { return CCString() << '(' << a << '/' << b << ')'; }
    };

    CCString customTypes;
    customTypes << "This is what a custom type could be displayed as:\n"
                << demoRational{1, 4} << ": demoRational{1, 4}\n";

    cout << builtInTypes << endl
         << customTypes << endl;
}

/**
 * Demonstrates the different manipulation functions CCString offers.
 */
void manipDemo()
{
    /**
     * supported operations:
     * - length()
     * - at(),[]
     * - append(),+=,<<,+
     * - replaceAt()
     * - insert()
     * - erase()
     * - trim()
     * - subString()
     * - find()
     * - findLast()
     */

    
}

/**
 * Shows ways of disecting strings using the built-in regex functions,
 * using a URL as an example.
 */
void regexDemo()
{
    /* 
     * We start with a string containing a URI:
     */
    CCString resource = "http://192.168.1.2:8080/index.html";

    CCString protocol = "http://";
    //regex describing a host address
    CCString hostRegex = "(\\d)+.(\\d)+.(\\d)+.(\\d)+";
    //regex describing an explicit port declaration
    CCString portRegex = ":(\\d)+";
    //regex describing a path declaration
    CCString pathRegex = "/(\\S)*";

    /* This regex matches a valid HTTP URL: */
    CCString validRegex = protocol + hostRegex + "(" + portRegex + ")?(" + pathRegex + ")?";

    /* First, we should check our link, using ::isMatch() */
    cout << "Checking URL \"" << resource << "\" for validity...\n";
    bool isValid = resource.isMatch(validRegex);

    if (!isValid)
        return;
    else
        cout << "URL valid.\n";

    /* 
     * If it meets our overall criteria, using ::containsMatch() we can check for its
     * optional components (host is defined to be mandatory, port and path are optional):
     * 
     * However, we need to mind that some regular expressions can match in multiple places!
     * In particular, pathRegex would match against the first slash in "http://" and the rest of the URL.
     * Therefore, we need to remove the protocol declaration.
     * 
     * Luckily, we have the methods ::replaceFirst and ::replaceAll, which, given two regular expressions,
     * replace the first or all occurrences, respectively, of the first regex with the second.
     */
    bool hasPort = resource.containsMatch(portRegex);
    bool hasPath = resource.replaceFirst(protocol, "").containsMatch(pathRegex);

    cout << "\nThe URL " << resource << " has " << (hasPort ? "a" : "no") << " port declaration and " << (hasPath ? "a" : "no") << " path declaration.\n";

    /* 
     * With that checked, we can extract each component with ::getMatch().
     */
    CCString host, port, path;

    //No checks, since we know that an IP address needs to be present.
    host = resource.getMatch(hostRegex);

    /* 
     * Note: We don't need to guard the call of ::getMatch() in this case
     * since ::getMatch() will return an empty string if no match exists.
     * 
     * We will guard the console output instead.
     */
    port = resource.getMatch(portRegex);
    path = resource.replaceFirst(protocol, "").getMatch(pathRegex);

    cout << "\nINFO: host=\"" << host << "\"; "
         << (hasPort ? "port=\"" + port + "\"; " : "")
         << (hasPath ? "path=\"" + path + "\"; " : "")
         << "\n";

    /* 
     * Another, seemingly more advanced functionality is the method used to split a string
     * at a defined delimiter (e.g. splitting a sentence into words):
     * 
     * CCString defines an own standard C++ iterator class to implement the feature,
     * the advantage being that neither CCStrings nor the user need to concern themselves with
     * storing an array (of any kind) of CCStrings, which would open up a plethora of questions about 
     * memory ownership and whether the user of this class wants to (or can) answer them.
     * By using an iterator, storing the strings is entirely optional.
     * 
     * To start splitting a string, call ::splitBegin() with the regex denoting the delimiter.
     * Increment the iterator to advance to the next partial string.
     * Compare against ::splitEnd() to know when you're finished.
     * 
     * Example: Splitting the IP address at the delimiting periods ('.')
     */
    cout << "\nComponents of host:\n";
    //   Don't bother typing out the type. Just don't.
    //   |    (the type is CCString::SplitIterator)
    //   V
    for (auto splIt = host.splitBegin("\\."); splIt != host.splitEnd(); ++splIt)
    {
        cout << "\"" << *splIt << "\"\n";
    }

    /* 
     * We can also pass them into the constructors of standard containers, 
     * and then iterate over those:
     */
    vector<CCString> hostComponents(host.splitBegin("\\."), host.splitEnd());

    uint_fast32_t ipAsBitMask = 0;

    for (size_t i = 0; i < hostComponents.size(); i++)
    {
        ipAsBitMask *= 256;
        //             using CCDecimal to convert the string to a number
        ipAsBitMask += CCDecimal(hostComponents[i]).toDouble();
    }
    cout << "the host address as a bitmask: \n"
         << std::bitset<32>(ipAsBitMask) << endl;

    return;
}

int main()
{
    cout << "\n---------------regexDemo: \n";
    regexDemo();

    cout << "\n---------------manipDemo: \n";
    manipDemo();

    cout << "\n---------------typesDemo: \n";
    typesDemo();
    return 0;
}