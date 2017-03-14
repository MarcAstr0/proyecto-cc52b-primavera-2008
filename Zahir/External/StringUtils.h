#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

/* *
 * FUENTE: http://www.codeproject.com/string/stringsplit.asp
 * Lo modifiqué para que cuando no encuentre el delimitador, entregue el string original.
 * Lo que hace esta clase es entregarme un vector con substrings del string que quiero separar por
 * cierto delimitador.
 * */

#include <string>
#include <vector>

class StringUtils {
public:
    static int SplitString(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool includeEmpties = true){
        int iPos = 0;
        int newPos = -1;
        int sizeS2 = (int)delimiter.size();
        int isize = (int)input.size();

        if (isize == 0 || sizeS2 == 0)
        	return 0;

        std::vector<int> positions;

        newPos = input.find (delimiter, 0);

        if (newPos < 0) { 
        	results.push_back(input);
            return 1; 
        }

        int numFound = 0;

        while (newPos >= iPos) {
            numFound++;
            positions.push_back(newPos);
            iPos = newPos;
            newPos = input.find (delimiter, iPos+sizeS2);
        }

        if (numFound == 0)
            return 0;

        for (int i=0; i <= (int) positions.size(); ++i) {
        	std::string s("");
            if (i == 0) 
                s = input.substr( i, positions[i] ); 
            int offset = positions[i-1] + sizeS2;
            
            if (offset < isize) {
                if (i == (int) positions.size())
                    s = input.substr(offset);
                else if (i > 0)
                    s = input.substr( positions[i-1] + sizeS2, positions[i] - positions[i-1] - sizeS2 );
            }
            if (includeEmpties || (s.size() > 0))
                results.push_back(s);
        }
        return numFound;
    };
};

#endif
