
#ifndef DRILL_H
#define DRILL_H

#include <map>
using std::map;

#include <string>
using std::string;

#include <vector>
using std::vector;
#include <map>
using std::map;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

extern "C" {
#include <gerbv.h>
}

#include "coord.hpp"

#include <boost/exception/all.hpp>
class drill_exception : virtual std::exception, virtual boost::exception {};

#include "mill.hpp"


class drillbit
{
public:
	double diameter;
	string unit;
	int drill_count;
};

//! Reads Excellon drill files and directly creates RS274-NGC gcode output.
/*! While we could easily add different input and output formats for the layerfiles
 *  to pcb2gcode, i've decided to ditch the importer/exporter scheme here.
 *  We'll very likely not encounter any drill files that gerbv can't read, and
 *  still rather likely never export to anything other than a ngc g-code file.
 *  Also, i'm lazy, and if I turn out to be wrong splitting the code won't be much effort anyway.
 */

class ExcellonProcessor
{
public:
	ExcellonProcessor( const string drillfile, const ivalue_t board_width );
	~ExcellonProcessor();

	void add_header( string );
	void export_ngc( const string ngcfile, shared_ptr<Driller> target, bool mirrored = true );

	shared_ptr<const map<int,drillbit> > get_bits();
	shared_ptr<const map<int,icoords> > get_holes();

private:
	void parse_holes();
	void parse_bits();

	const ivalue_t board_width;

	shared_ptr< map<int,drillbit> > bits;
	shared_ptr< map<int,icoords> > holes;

	gerbv_project_t* project;

	vector<string> header;
};


#endif // DRILL_H