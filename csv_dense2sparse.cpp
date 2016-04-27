#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;
void exit_with_help();

std::string prelog("[csv2pajek]:: ");
typedef struct
{
    float threshold;
    bool useModulo;
    bool binarized;
    char *inputfilename;
    char *outputfilename;
    int weightsprecision;
}
Parameters;

Parameters param;

/**
 * @brief parse_command_line
 * @param argc
 * @param argv
 */
void parse_command_line(int argc, char **argv)
{
    // Parse options
    for( int i=1; i<argc; i++ )
    {
        if( argv[i][0] != '-' )
        {
            break;
        }

        if( ++i>=argc )
        {
            exit_with_help();
        }
        switch(argv[i-1][1])
        {
        case 'p':
        {
            param.weightsprecision=atoi(argv[i]);
            break;
        }
        case 't':
        {
            param.threshold=atof(argv[i]);
            break;
        }
        case 'b':
        {
            param.binarized=(bool)atoi(argv[i]);
            break;
        }
        case 'a':
        {
            param.useModulo=(bool)atoi(argv[i]);
            break;
        }
        case 'i':
        {
            param.inputfilename=argv[i];
            break;
        }
        case 'o':
        {
            param.outputfilename=argv[i];
            break;
        }
        case 'h':
        {
            exit_with_help();
        }
        default:
            fprintf(stderr,"Unknown option: -%c\n", argv[i-1][1]);
            exit_with_help();
        }
    }
}

/**
 * @brief exit_with_help
 */
void exit_with_help()
{
    fprintf(stderr,
            "Usage: csv_dense_to_matrix_to_sparse_net -i [input_file] [options] -o [output_file]\n"
            "options:\n"
            "-t threshold\n"
            "   Sets the threshold to filter out arcs\n"
            "-a\n"
            "   0 Don't use absolute module\n"
            "   1 Use absolute module\n"
            "-b\n"
            "   0 Don't binarize network\n"
            "   1 Binarize network\n"
            "-p precision\n"
            "Load a csv file matrix.csv and convert to Pajek with 5 digits precision for weights\n"
            "Example: ./csv_dense_to_matrix_to_sparse_net -i matrix.csv -t 0.4 -a 1 -p 5 -o thresholded_graph_matrix.net\n"
            );
    exit(1);
}


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
    if (argc==1)
        exit_with_help();
    param.threshold=0.5;
    param.useModulo=false;
    param.weightsprecision=5;

    parse_command_line(argc,argv);
    cerr << prelog << "Converting to .net using t= " << param.threshold << " modulo= " << param.useModulo << " precision= " << param.weightsprecision << " binarization= " << (param.binarized ? std::string("Yes") :std::string("No")) << endl;
    std::string filename(param.inputfilename);
    string line;
    line.reserve(1E7);
    ifstream is(filename.c_str());
    if (is==NULL)
    {
        cerr << prelog << "File " << filename <<  "does not exists, select a file" << endl;
        std::exit(0);
    }
    float threshold=param.threshold;
    const char separator=' ';

    bool useModulo = param.useModulo;

    int nrows = 0;
    int ncols = 0;
    // First let's count the rows of the file
    while (std::getline(is,line))
        nrows++;

    // Return the pointer of file to beginning
    is.clear();
    is.seekg(0, ios::beg);
    // Then count the number of columns
    std::getline(is,line);
    std::istringstream iss(line);
    std::string result;
    while( std::getline( iss, result, separator ) )
        ++ncols;

    // Return the pointer of file to beginning
    is.clear();
    is.seekg(0, ios::beg);

    cerr << prelog << "nrows= " << nrows << "\n" << prelog << "ncols= " << ncols << endl;
    cerr << prelog << "Reading matrix, saving thresholded...";

    ofstream outputfile;
    try
    {
        outputfile.open(param.outputfilename);
        outputfile << "*Vertices " << nrows << endl;
        for (unsigned int r=0; r<nrows; ++r)
        {
            outputfile << r+1 << " \"\"" << endl;
        }

        outputfile << endl << "*Edgeslist" << endl;
    }
    catch (const std::exception &e)
    {
        cerr << "Cant open file " << param.outputfilename << "\t" << e.what() << endl;
    }
    cerr << endl;
    outputfile << std::setprecision(param.weightsprecision) << endl;
    unsigned int rowIndex=1;
    line.clear();
    line.reserve(1E7);
    unsigned int nEdges=0;
    while ( std::getline(is,line) )
    {
        fprintf(stderr,"Conversion %s %2.2f - Current Edges %d - Current density (percent) %2.3g \r",prelog.c_str(), (float)rowIndex/nrows*100, nEdges, 100.0f*(float)nEdges/(nrows*(nrows-1)) );
        std::istringstream iss(line);
        double val;
        unsigned int colIndex=1;
        outputfile << rowIndex << " ";
        while ( iss >> val )
        {
            if ( useModulo )
            {
                double absval=abs(val);
                if ( absval >= threshold && colIndex!=rowIndex)
                {
                    ++nEdges;
                    if (param.binarized)
                        outputfile << colIndex << " ";
                    else
                        outputfile << colIndex << ":" << absval << " " ;
                }
            }
            else
            {
                if ( val >= threshold && colIndex!=rowIndex)
                {
                    ++nEdges;
                    if (param.binarized)
                        outputfile << colIndex << " ";
                    else
                        outputfile << colIndex << ":" << val << " " ;
                }
            }
            ++colIndex;
        }

        outputfile << endl;
        ++rowIndex;
    }
    cerr << "done" << endl;


    return 0;
}
