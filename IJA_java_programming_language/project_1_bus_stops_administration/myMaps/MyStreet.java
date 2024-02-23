package vut.fit.ija.homework1.myMaps;

import vut.fit.ija.homework1.maps.Coordinate;
import vut.fit.ija.homework1.maps.Stop;
import vut.fit.ija.homework1.maps.Street;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class MyStreet implements Street {
    String name;
    Coordinate coor1;
    Coordinate coor2;
    List<Stop> Stops;
    public String getId() {
        return name;
    }

    public List<Coordinate> getCoordinates() {
        List<Coordinate>Coor = new ArrayList<>();
        Coor.add(coor1);
        Coor.add(coor2);
        return Coor;
    }

    public List<Stop> getStops() {
        return Stops;
    }

    public void addStop(Stop stop){
        Stops.add(stop);
        stop.setStreet(this);
    }

    public MyStreet(String name, Coordinate coor1, Coordinate coor2){
        this.name = name;
        this.coor1 = coor1;
        this.coor2 = coor2;
        Stops = new ArrayList<>();
    }
}
