using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using FestivalTracker.Data;
using Microsoft.EntityFrameworkCore;

namespace FestivalTracker.Services {
    public class TicketManager {
        private DatabaseContext _database;
        
        public TicketManager(DatabaseContext databaseContext) {
            _database = databaseContext;
        }
        
        public List<(TicketType type, int remaining)> GetTickets(Festival f) {
            var now = DateTime.Now;
            var result = new List<(TicketType, int)>();
            var reservations = _database.Reservation.Include(x=>x.Ticket).Where(x => x.ExpirationDate < now);
            foreach (var item in reservations) {
                _database.RemoveRange(item.Ticket);
            }
            _database.SaveChanges();
            _database.RemoveRange(reservations.ToList());
            _database.SaveChanges();
            var tickets = _database.Ticket.ToList();
            foreach (var type in _database.TicketType.Where(x => x.Festival.Id == f.Id)) {
                var boughtCount = tickets.Count(x => x.TicketTypeId == type.Id);
                result.Add((type, type.Count - boughtCount));
            }
            return result;
        }

        public int GetRemainingCount(TicketType t) {
            var now = DateTime.Now;
            var reservations = _database.Reservation.Include(x=>x.Ticket).Where(x => x.ExpirationDate < now);
            foreach (var item in reservations) {
                _database.RemoveRange(item.Ticket);
            }
            _database.SaveChanges();
            _database.RemoveRange(reservations);
            _database.SaveChanges();
            _database.RemoveRange(_database.Reservation.Where(x => x.ExpirationDate < now));
            var tickets = _database.Ticket.ToList();
            return t.Count - tickets.Count(x => x.TicketType.Id == t.Id);

        }
        
        public Reservation CreateReservation(int personId, TicketType type, int count, TimeSpan expiration) {
            var reservation = new Reservation {
                State = "Rezervováno",
                PersonId = personId,
                ExpirationDate = DateTime.Now + expiration
            };
            _database.Reservation.Add(reservation);
            _database.SaveChanges();
            for (var i = 0; i < count; i++) {
                var t = new Ticket {
                    ReservationId = reservation.Id,
                    FestivalId = type.FestivalId,
                    TicketTypeId = type.Id
                }; 
                _database.Ticket.Add(t);
                reservation.Ticket.Add(t);
            }
            _database.Reservation.Update(reservation);
            _database.SaveChanges();
            return reservation;
        }


        public void RemoveReservation(Reservation r) {
            _database.Ticket.RemoveRange(r.Ticket);
            _database.Reservation.Remove(r);
            _database.SaveChanges();
        }
        
        
        
        public void PayForReservation(Reservation r){
            r.ExpirationDate = DateTime.MaxValue;
            r.State = "Zaplaceno";
            _database.Reservation.Update(r);
            _database.SaveChanges();
        }
    }
}