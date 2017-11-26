#ifndef EVENTPROPAGATION_H
#define EVENTPROPAGATION_H

#include <type_traits>
#include <map>
#include <set>
#include <list>
#include <vector>

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/List.h"
#include "Bang/Array.h"

template <typename T> struct IsContainer : std::false_type {} ;
template <typename... Args> struct IsContainer< std::map<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< std::set<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< std::list<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< std::vector<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< Bang::Map<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< Bang::Set<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< Bang::List<Args...> > : std::true_type{};
template <typename... Args> struct IsContainer< Bang::Array<Args...> > : std::true_type{};

#endif // EVENTPROPAGATION_H
