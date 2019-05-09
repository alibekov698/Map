/*
 * set.cpp
 *
 *  Created on: Mar 18, 2019
 *      Author: benja
 */

#include "map.h"

// From previous task:

bool equal( const std::string& s1, const std::string& s2 )
{
    if(s1.size() != s2.size())
    {
        return false;
    }
    else
    {
        for( size_t i = 0; i < s1.size(); i++ )
        {
            if(tolower(s1[i]) != tolower(s2[i]))
            {
                return false;
            }
        }
        return true;
    }
}

size_t hash( const std::string& st )
{
    size_t val = 0;

    for (size_t i = 0; i < st.size(); i++)
    {
        val = int(tolower(st[i])) + val;
    }
    return val;
}

map::listofpairs::const_iterator
map::find( const listofpairs& lst, const std::string& key )
{
    auto p = lst. begin( );
    while( p != lst. end( ) && !equal( p -> first, key ))
    {
        ++ p;
    }
    return p;
}

map::listofpairs::iterator
map::find( listofpairs& lst, const std::string& key )
{
    auto p = lst. begin( );
    while( p != lst. end( ) && !equal( p -> first, key ))
    {
        ++ p;
    }
    return p;
}

bool map::contains_key( const std::string& key ) const
{
    listofpairs bucket = getbucket(key);
    return find(bucket, key) != bucket.end();
}

bool map::insert( const std::string& key, unsigned int val )
{
    if( find(getbucket(key), key) == getbucket(key).end() )
    {
        map_size ++;
        getbucket(key).push_back( std::pair <std::string, unsigned int> (key, val));
        return true;
    }
    else
    {
        return false;
    }
}

unsigned int& map::operator[] ( const std::string& key )
{
    if ( find(getbucket(key), key) != getbucket(key).end() )
    {
        return (find(getbucket(key), key)) -> second;
    }
    else
    {
        insert(key, 0);
        return (find(getbucket(key), key)) -> second;
    }
}

unsigned int& map::at( const std::string& key )
{
    if ( find(getbucket(key), key) != getbucket(key).end() )
    {
        return (find(getbucket(key), key)) -> second;
    }
    else
    {
        throw std::out_of_range( "at( ): string not found" );
    }
}

unsigned int map::at( const std::string& key ) const
{
    if ( find(getbucket(key), key) != getbucket(key).end() )
    {
        return (find(getbucket(key), key)) -> second;
    }
    else
    {
        throw std::out_of_range( "at( ): string not found" );
    }
}

void map::rehash( size_t newbucketsize )
{
    if( newbucketsize < 4 )
    {
        newbucketsize = 4;
    }
    std::vector< listofpairs > newbuckets{ newbucketsize };

    for (size_t j = 0; j < buckets.size(); j++)
    {
        for ( auto it = buckets[j].begin(); it != buckets[j].end(); it++ )
        {
            size_t new_index = hash(it -> first) % newbuckets.size();
            newbuckets[new_index].push_back(*it);
        }
    }
    buckets = newbuckets;
}

void map::check_rehash( )
{
    if (loadfactor() > max_load_factor)
    {
        rehash(buckets.size() * 2);
    }
}

bool map::remove( const std::string& key )
{
    size_t index = hash(key) % buckets.size();

    for( auto it = buckets[index].begin(); it != buckets[index].end(); it++ )
    {
        if ( equal(it -> first, key) )
        {
            buckets[index].erase(it);
            map_size --;
            return true;
        }
    }
    return false;
}

double map::standarddev( ) const
{
    double sum = 0.0;
    double lf = loadfactor();

    for ( const auto& l : buckets )
    {
        double dif = l.size() - lf;
        sum += dif*dif;
    }
    return sqrt( sum / buckets. size( ));
}

void map::clear( )
{
    for( auto& l : buckets )
    {
        l. clear( );
    }
    map_size = 0;
}

std::ostream& map::print( std::ostream& out ) const
{
    for (size_t j = 0; j < buckets.size(); j++)
    {
        out << "bucket[" << j << "]:    { ";
        int size_of_single_bucket = buckets[j].size();

        for ( auto it = buckets[j].begin(); it != buckets[j].end(); it++ )
        {
            size_of_single_bucket--;
            out << it -> first << "/" << it -> second;
            if(size_of_single_bucket != 0)
            {
                out << ", ";
            }
        }
        out << " }\n";
    }
    return out;
}

std::ostream& map::printstatistics( std::ostream& out ) const
{
    out << "set size =            " << size( ) << "\n";
    out << "load factor =         " << loadfactor( ) << " ( max = " <<
        max_load_factor << " )\n";
    out << "standard deviation =  " << standarddev( ) << "\n";
    return out;
}
