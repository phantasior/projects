namespace Models.Accounts;

public record Account(long Id, string PinHash, long UserId, long Balance);