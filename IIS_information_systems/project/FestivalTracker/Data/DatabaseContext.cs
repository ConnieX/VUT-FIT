using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;

namespace FestivalTracker.Data
{
    public partial class DatabaseContext : DbContext
    {
        public DatabaseContext()
        {
        }

        public DatabaseContext(DbContextOptions<DatabaseContext> options) : base(options) {}

        public virtual DbSet<Artist> Artist { get; set; }
        public virtual DbSet<Concert> Concert { get; set; }
        public virtual DbSet<Festival> Festival { get; set; }
        public virtual DbSet<Performer> Performer { get; set; }
        public virtual DbSet<PerformerHasArtist> PerformerHasArtist { get; set; }
        public virtual DbSet<Person> Person { get; set; }
        public virtual DbSet<Reservation> Reservation { get; set; }
        public virtual DbSet<Ticket> Ticket { get; set; }
        public virtual DbSet<TicketType> TicketType { get; set; }
        public virtual DbSet<User> User { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            if (!optionsBuilder.IsConfigured)
            {
                optionsBuilder.UseSqlServer("Name=DbConnection");
            }
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Artist>(entity =>
            {
                entity.ToTable("Artist", "festival_schema");

                entity.HasIndex(e => e.Id)
                    .HasName("ID_UNIQUE_ARTIST")
                    .IsUnique();

                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.Property(e => e.Surname)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);
            });

            modelBuilder.Entity<Concert>(entity =>
            {
                entity.HasKey(e => new { e.FestivalId, e.PerformerId })
                    .HasName("PK__Concert__AA5E09FBB8B9C1C8");

                entity.ToTable("Concert", "festival_schema");

                entity.HasIndex(e => e.FestivalId)
                    .HasName("fk_Festival_has_Performer_Festival1_idx");

                entity.HasIndex(e => e.PerformerId)
                    .HasName("fk_Festival_has_Performer_Performer1_idx");

                entity.Property(e => e.FestivalId).HasColumnName("FestivalID");

                entity.Property(e => e.PerformerId).HasColumnName("PerformerID");

                entity.Property(e => e.End).HasColumnType("datetime2(0)");

                entity.Property(e => e.Start).HasColumnType("datetime2(0)");

                entity.HasOne(d => d.Festival)
                    .WithMany(p => p.Concert)
                    .HasForeignKey(d => d.FestivalId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_Festival_has_Performer_Festival1");

                entity.HasOne(d => d.Performer)
                    .WithMany(p => p.Concert)
                    .HasForeignKey(d => d.PerformerId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_Festival_has_Performer_Performer1");
            });

            modelBuilder.Entity<Festival>(entity =>
            {
                entity.ToTable("Festival", "festival_schema");

                entity.HasIndex(e => e.Id)
                    .HasName("ID_UNIQUE_FESTIVAL")
                    .IsUnique();

                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.CreatorId).HasColumnName("CreatorID");

                entity.Property(e => e.Details).IsUnicode(false);

                entity.Property(e => e.EndDate).HasColumnType("date");

                entity.Property(e => e.Genre)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.Property(e => e.Geolocation)
                    .HasMaxLength(100)
                    .IsUnicode(false);

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.Property(e => e.Place)
                    .IsRequired()
                    .HasMaxLength(100)
                    .IsUnicode(false);

                entity.Property(e => e.StartDate).HasColumnType("date");

                entity.HasOne(d => d.Creator)
                    .WithMany(p => p.Festival)
                    .HasForeignKey(d => d.CreatorId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("Festival_User_PersonID_fk");
            });

            modelBuilder.Entity<Performer>(entity =>
            {
                entity.ToTable("Performer", "festival_schema");

                entity.HasIndex(e => e.Id)
                    .HasName("ID_UNIQUE_Performer")
                    .IsUnique();

                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.Genre)
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.Property(e => e.Logo)
                    .HasMaxLength(100)
                    .IsUnicode(false);

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);
            });

            modelBuilder.Entity<PerformerHasArtist>(entity =>
            {
                entity.HasKey(e => new { e.PerformerId, e.ArtistId })
                    .HasName("PK__Performe__6D22C04C66141F2A");

                entity.ToTable("Performer_has_Artist", "festival_schema");

                entity.HasIndex(e => e.ArtistId)
                    .HasName("fk_Performer_has_Artist_Artist1_idx");

                entity.HasIndex(e => e.PerformerId)
                    .HasName("fk_Performer_has_Artist_Performer1_idx");

                entity.Property(e => e.PerformerId).HasColumnName("Performer_ID");

                entity.Property(e => e.ArtistId).HasColumnName("Artist_ID");

                entity.HasOne(d => d.Artist)
                    .WithMany(p => p.PerformerHasArtist)
                    .HasForeignKey(d => d.ArtistId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_Performer_has_Artist_Artist1");

                entity.HasOne(d => d.Performer)
                    .WithMany(p => p.PerformerHasArtist)
                    .HasForeignKey(d => d.PerformerId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_Performer_has_Artist_Performer1");
            });

            modelBuilder.Entity<Person>(entity =>
            {
                entity.ToTable("Person", "festival_schema");

                entity.HasIndex(e => e.Id)
                    .HasName("id_UNIQUE")
                    .IsUnique();

                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.Birth).HasColumnType("date");

                entity.Property(e => e.Email)
                    .IsRequired()
                    .HasMaxLength(75)
                    .IsUnicode(false);

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.Property(e => e.Surname)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);
            });

            modelBuilder.Entity<Reservation>(entity =>
            {
                entity.HasKey(e => e.Id)
                    .HasName("Reservation_pk")
                    .IsClustered(false);

                entity.ToTable("Reservation", "festival_schema");

                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.ExpirationDate).HasColumnType("datetime");

                entity.Property(e => e.PersonId).HasColumnName("PersonID");

                entity.Property(e => e.State)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.HasOne(d => d.Person)
                    .WithMany(p => p.Reservation)
                    .HasForeignKey(d => d.PersonId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_Reservation_Person1");
            });

            modelBuilder.Entity<Ticket>(entity =>
            {
                entity.HasKey(e => new { e.Id, e.TicketTypeId, e.FestivalId, e.ReservationId })
                    .HasName("PK__Ticket__6805A7F580490990");

                entity.ToTable("Ticket", "festival_schema");

                entity.HasIndex(e => e.Id)
                    .HasName("ID_UNIQUE_TICKET")
                    .IsUnique();

                entity.HasIndex(e => e.ReservationId)
                    .HasName("fk_Ticket_Reservation1_idx");

                entity.HasIndex(e => new { e.TicketTypeId, e.FestivalId })
                    .HasName("fk_Ticket_TicketType1_idx");

                entity.Property(e => e.Id)
                    .HasColumnName("ID")
                    .ValueGeneratedOnAdd();

                entity.Property(e => e.TicketTypeId).HasColumnName("TicketTypeID");

                entity.Property(e => e.FestivalId).HasColumnName("FestivalID");

                entity.Property(e => e.ReservationId).HasColumnName("ReservationID");

                entity.HasOne(d => d.Reservation)
                    .WithMany(p => p.Ticket)
                    .HasForeignKey(d => d.ReservationId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("Ticket_Reservation_ID_fk");

                entity.HasOne(d => d.TicketType)
                    .WithMany(p => p.Ticket)
                    .HasForeignKey(d => new { d.TicketTypeId, d.FestivalId })
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_Ticket_TicketType1");
            });

            modelBuilder.Entity<TicketType>(entity =>
            {
                entity.HasKey(e => new { e.Id, e.FestivalId })
                    .HasName("PK__TicketTy__FA613B0D17FDF549");

                entity.ToTable("TicketType", "festival_schema");

                entity.HasIndex(e => e.FestivalId)
                    .HasName("fk_TicketType_Festival1_idx");

                entity.HasIndex(e => e.Id)
                    .HasName("ID_UNIQUE_TICKET_TYPE");

                entity.Property(e => e.Id)
                    .HasColumnName("ID")
                    .ValueGeneratedOnAdd();

                entity.Property(e => e.FestivalId).HasColumnName("FestivalID");

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasMaxLength(45)
                    .IsUnicode(false);

                entity.Property(e => e.ValidFrom).HasColumnType("date");

                entity.Property(e => e.ValidTo).HasColumnType("date");

                entity.HasOne(d => d.Festival)
                    .WithMany(p => p.TicketType)
                    .HasForeignKey(d => d.FestivalId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_TicketType_Festival1");
            });

            modelBuilder.Entity<User>(entity =>
            {
                entity.HasKey(e => e.PersonId)
                    .HasName("PK__User__AA2FFB85C7445BE6");

                entity.ToTable("User", "festival_schema");

                entity.Property(e => e.PersonId)
                    .HasColumnName("PersonID")
                    .ValueGeneratedNever();

                entity.Property(e => e.Address)
                    .HasMaxLength(100)
                    .IsUnicode(false);

                entity.Property(e => e.Password)
                    .HasMaxLength(256)
                    .IsUnicode(false);

                entity.Property(e => e.ProfilePic)
                    .HasMaxLength(100)
                    .IsUnicode(false);

                entity.HasOne(d => d.Person)
                    .WithOne(p => p.User)
                    .HasForeignKey<User>(d => d.PersonId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("fk_User_Person");
            });

            OnModelCreatingPartial(modelBuilder);
        }

        partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
    }
}
