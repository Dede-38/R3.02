/* 
 * File:   Mondial.cpp
 * Author: hb
 * 
 * Created on 22 novembre 2018, 16:05
 */

#include "Mondial.h"


#include <iostream>     // pour cout
#include <iomanip>      // pour setw()
#include <sstream>
#include <iterator>
#include <utility>

Mondial::Mondial(const char *filename) {
    // Chargement du fichier XML en mémoire
    imageMondial.LoadFile(filename);
    // Initialisation de l'attribut racineMondial avec la racine (élément <mondial>)
    racineMondial = imageMondial.FirstChildElement();
}

void Mondial::Print() {
    imageMondial.Print();
}

/*
 * FOURNIE
 */
int Mondial::getNbAirports() const {
    // initialisation du nombre d’aéroports
    int nb = 0;
    // accéder à <airportscategory>, c’est un fils de l'élément <racineMondial>
    XMLElement *airportsCategory = racineMondial->FirstChildElement("airportscategory");
    // parcours complet des fils de <airportscategory> en les comptants
    // 1) accéder au premier fils <airport> de <airportscategory>
    XMLElement *currentAirport = airportsCategory->FirstChildElement();
    // 2) parcourir tous les <airport> qui sont des frères
    while (currentAirport != nullptr) {
        // un aéroport supplémentaire
        nb = nb + 1;
        // avancer au frère <airport> suivant de currentAirport
        currentAirport = currentAirport->NextSiblingElement();
    }
    // currentAirport n’a plus de frère {currentAirport == nullptr}, c’est le dernier
    return nb;
}

/*
 * FOURNIE
 */
void Mondial::printCountriesCode() const {
    int rank = 1; // rang du pays
    string carcodeValue; // valeur de l'attribut "car_cod" du pays courant
    // accéder à <countriescategory>, c’est un fils de l'élément <racineMondial>)
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // parcours complet des fils de <countriescategory> en affichant le rang et le code
    // 1) accéder au premier fils <country> de <countriescategory>
    XMLElement *currentCountry = countriesCategory->FirstChildElement();
    // 2) parcourir tous les <country> qui sont des frères
    while (currentCountry != nullptr) {
        // traiter le pays courant
        //      1) récupérer la valeur de l’attribut "car_code"
        carcodeValue = currentCountry->Attribute("car_code");
        //      2) faire l’affichage
        cout << setw(5) << rank << " : " << carcodeValue << endl;
        // avancer au frère <country> suivant de currentCountry
        currentCountry = currentCountry->NextSiblingElement();
        // mettre à jour le rang
        rank = rank + 1;
    }
    // currentCountry n’a pas de frère {currentCountry == nullptr}, c’est fini
}


/*
 * A COMPLETER
 */
int Mondial::getNbDeserts() const {
    int nb = 0;
    XMLElement *desertsCategorie = racineMondial->FirstChildElement("desertscategory");
    XMLElement *desertActuel = desertsCategorie->FirstChildElement();
    while (desertActuel != nullptr) {
        nb = nb + 1;
        desertActuel = desertActuel->NextSiblingElement();
    }
    return nb;
}

/*
 * A COMPLETER
 */
int Mondial::getNbElemCat(const string categoryName) {
    int nombre = 0;
    auto nomcomplet = decod_category.find(categoryName);
    if (nomcomplet != decod_category.end()) {
        string NomFichierComplet = nomcomplet->second;
        XMLElement *element = racineMondial->FirstChildElement(NomFichierComplet.c_str());
        if (element) {
            XMLElement *ElementCourant = element->FirstChildElement();
            while (ElementCourant != nullptr) {
                nombre++;
                ElementCourant = ElementCourant->NextSiblingElement();
            }
        }
    }
    return nombre;
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getCountryXmlelementFromNameRec(string countryName) const {
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    return getCountryXmlelementFromNameRecWorker(countriesCategory->FirstChildElement(), countryName);
}

// Méthode auxiliaire récursive
XMLElement *Mondial::getCountryXmlelementFromNameRecWorker(XMLElement *currentCountry, string countryName) const {
    if (currentCountry == nullptr) {
        return nullptr;
    }
    const char *nompays = currentCountry->FirstChildElement("name")->GetText();
    if (nompays == countryName) {
        return currentCountry;
    }
    return getCountryXmlelementFromNameRecWorker(currentCountry->NextSiblingElement("country"), countryName);
}

string Mondial::getCountryCodeFromName(string countryName) const throw (PrecondVioleeExcep) {
    XMLElement *countryElement = getCountryXmlelementFromNameRec(countryName);
    if (countryElement) {
        return countryElement->Attribute("car_code");
    }
    throw PrecondVioleeExcep(
        "Exception pour précondition violée : Dans getCountryCodeFromName, le pays " + countryName + " n'existe pas !");
}

/*
 * A COMPLETER
 */
/**
 * élément <country> d'un pays identifié par son nom countryName
 * @param countryName
 * @return pointeur sur l'élément <country> dont la valeur du fils <name> est égal à countryName, nullprt sinon
 */
XMLElement *Mondial::getCountryXmlelementFromNameIter(string countryName) const {
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    XMLElement *currentCountry = countriesCategory->FirstChildElement();
    if (currentCountry != nullptr) {
        // parcourir les pays tant que le nom ≠ de countryName
        while ((currentCountry != nullptr) && (currentCountry->FirstChildElement("name")->GetText() != countryName)) {
            currentCountry = currentCountry->NextSiblingElement("country");
        }
        return currentCountry;
    }
    return nullptr;
}

/*
 * A COMPLETER
 */
int Mondial::getCountryPopulationFromName(string countryName) const {
    int population = 0;
    XMLElement *countryElement = getCountryXmlelementFromNameRec(countryName);
    if (countryElement) {
        population = atoi(countryElement->LastChildElement("population")->GetText());
        return population;
    }
    return -1;
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getCountryXmlelementFromCode(string countryCode) const {
    /*
     * A COMPLETER
     */
    // supprimer à partir d'ici après complétion
    return nullptr;
}

/*
 * A COMPLETER
 */
void Mondial::printCountryBorders(string countryName) const {
    string PaysFrontalier;
    XMLElement *countryElement = getCountryXmlelementFromNameIter(countryName);
    XMLElement *Frontières = countryElement->FirstChildElement("border");

    if (Frontières == nullptr) {
        cout << "Le pays " + countryName + " ne possède aucune frontière" << endl;
    }
    while (Frontières != nullptr) {
        PaysFrontalier = Frontières->Attribute("country");
        float Distance = atof(Frontières->Attribute("length"));

        cout << "Pour le Pays " << countryName + " les pays Frontaliers sont " << PaysFrontalier <<
                " avec une distance de " << Distance << "km" << endl;
        // avancer au frère <country> suivant de currentCountry
        Frontières = Frontières->NextSiblingElement("border");
    }
    cout << <<endl;aaaa
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getRiverXmlelementFromNameIter(string riverName) const {
    return nullptr;
}

/*
 * A COMPLETER
 */
void Mondial::printAllCountriesCrossedByRiver(string riverName) const {
    string PaysFrontalier;
    XMLElement *countryElement = getCountryXmlelementFromNameIter(riverName);
    XMLElement *Frontières = countryElement->FirstChildElement("border");

    if (countryElement == nullptr) {
        cout << "Erreur le pays n'existe pas" << endl;
    }
    if (Frontières == nullptr) {
        cout << "Le pays " + riverName + " ne possède aucune frontière" << endl;
    }
    while (Frontières != nullptr) {
        PaysFrontalier = Frontières->Attribute("country");
        float Distance = atof(Frontières->Attribute("length"));

        cout << "Pour le Pays " << riverName + " les pays Frontaliers sont " << PaysFrontalier <<
                " avec une distance de " << Distance << "km" << endl;
        // avancer au frère <country> suivant de currentCountry
        Frontières = Frontières->NextSiblingElement("border");
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesWithProvincesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
}

/*
 * A COMPLETER
 */
void Mondial::printCityInformation(string cityName) const {
    /*
     * A COMPLETER
     */
}

/**
 * Exemple de question additionnelle pour l'exercice 9 afficher toutes les informations disponibles
 * dans Mondial concernant toutes les îles.
 * On peut commencer par une île en particulier à partir de son nom
 */
void Mondial::printIslandsInformations() const {
    /*
     * A COMPLETER
     */
}

/*
 * Méthodes de service fournies
 */

template<typename Out>
void Mondial::split(string &s, char delim, Out result) const {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

/**
 * Méthode à utiliser pour découper les mots d'une chaîne dans un vecteur
 * Utile pour la gestion des attributs à valeurs multiples
 * @param s chaîne à découper
 * @param delim délimiteur des mots de la chaîne à découper
 * @return vecteur contenant les mots de la chaîne découpée
 */
vector<std::string> Mondial::split(string &s, char delim) const {
    vector<std::string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

Mondial::~Mondial() {
}
