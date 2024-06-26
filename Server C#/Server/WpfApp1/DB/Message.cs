﻿using Microsoft.EntityFrameworkCore;

namespace SQLiteApp
{
    public class Message
    {
        public int Id { get; set; }
        public string Text { get; set; }

    }
    public class ApplicationContext : DbContext
    {

        public DbSet<Message> messages { get; set; } = null!;
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Data Source=messages.db");
        }
    }
}
