namespace Models.Users;

public record User(long Id, string Username, string Password, UserRole Role);