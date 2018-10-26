// LocationFilter.h

#ifndef LOCATION_FILTER_H_
#define LOCATION_FILTER_H_

#include <list>
#include <set>
#include <string>

class DataNode;
class DataWriter;
class Government;
class Planet;
class Ship;
class System;



// This class represents a set of constraints on a randomly chosen planet or
// system. For example, it can require that the planet used for a mission have
// a certain attribute or be owned by a certain government, or be a certain
// distance away from the current system.
class LocationFilter {
public:
  LocationFilter() = default;
  // Construct and Load() at the same time.
  LocationFilter(const DataNode &node);
  
  // Examine all the children of the given node and load any that are filters.
  void Load(const DataNode &node);
  // This only saves the children. Save the root node separately. It does
  // handle indenting, however.
  void Save(DataWriter &out) const;
  
  // Check if this filter contains any specifications.
  bool IsEmpty() const;
  
  // If the player is in the given system, does this filter match?
  bool Matches(const Planet *planet, const System *origin = nullptr) const;
  bool Matches(const System *system, const System *origin = nullptr) const;
  bool Matches(const Ship &ship) const;
  
  // Return a new LocationFilter with any "distance" conditions converted
  // into "near" references, relative to the given system.
  LocationFilter SetOrigin(const System *origin) const;
  // Generic find system / find planet methods, based on the given origin
  // system (e.g. the player's current system) and ability to land.
  const System *PickSystem(const System *origin) const;
  const Planet *PickPlanet(const System *origin, bool hasClearance = false) const;
  
  
private:
  // Load one particular line of conditions.
  void LoadChild(const DataNode &child);
  // Check if the filter matches the given system. If it did not, return true
  // only if the filter wasn't looking for planet characteristics or if the
  // didPlanet argument is set (meaning we already checked those).
  bool Matches(const System *system, const System *origin, bool didPlanet) const;
  
  
private:
  // The planet must satisfy these conditions:
  std::set<const Planet *> planets;
  // It must have at least one attribute from each set in this list:
  std::list<std::set<std::string>> attributes;
  
  // The system must satisfy these conditions:
  std::set<const System *> systems;
  std::set<const Government *> governments;
  // The reference point and distance limits of a "near <system>" filter.
  const System *centre = nullptr;
  int centreMinDistance = 0;
  int centreMaxDistance = 1;
  // Distance limits used in a "distance" filter.
  int originMinDistance = 0;
  int originMaxDistance = -1;
  
  // These filters store all the things the planet or system must not be.
  std::list<LocationFilter> notFilters;
  // These filters store all the things the planet or system must border.
  std::list<LocationFilter> neighborFilters;
};



#endif
