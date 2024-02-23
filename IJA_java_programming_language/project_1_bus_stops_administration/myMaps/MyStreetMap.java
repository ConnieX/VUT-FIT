package vut.fit.ija.homework1.myMaps;

import vut.fit.ija.homework1.maps.Street;
import vut.fit.ija.homework1.maps.StreetMap;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class MyStreetMap implements StreetMap {
    List<Street> Streets;

    public void addStreet(Street s) {
        Streets.add(s);
    }

    public Street getStreet(String id) {
        for (Street street:Streets){
            if (street.getId() == id){
                return street;
            }
        }
        return null;
    }


    public MyStreetMap(){
        Streets = new ArrayList<>();
    }
}
