package vut.fit.ija.homework1.myMaps;

import vut.fit.ija.homework1.maps.Coordinate;
import vut.fit.ija.homework1.maps.Stop;
import vut.fit.ija.homework1.maps.Street;

import java.util.Objects;

public class MyStop implements Stop {
    String name;
    Coordinate coor;
    Street strname;
    public String getId() {
        return name;
    }

    public Coordinate getCoordinate() {
        return coor;
    }

    public void setStreet(Street s) {
        strname = s;
    }

    public Street getStreet() {
        return strname;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyStop myStop = (MyStop) o;
        return Objects.equals(name, myStop.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, coor, strname);
    }

    public MyStop(String name, Coordinate coor){
        this.name = name;
        this.coor = coor;
    }

    public MyStop(String name){
        this.name = name;
    }
}
