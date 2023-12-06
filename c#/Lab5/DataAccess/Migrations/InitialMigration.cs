using FluentMigrator;
using FluentMigrator.Expressions;
using FluentMigrator.Infrastructure;

namespace DataAccess.Migrations;

[Migration(9, "never give up")]
public class InitialMigration : IMigration
{
    public object ApplicationContext => throw new NotSupportedException();

    public string ConnectionString => throw new NotSupportedException();

    public void GetUpExpressions(IMigrationContext context)
    {
        ArgumentNullException.ThrowIfNull(context);
        const string sql =
            """
                create type user_role as enum
                (
                    'admin',
                    'user'
                );            

                create table users
                (
                    id bigint primary key generated always as identity,
                    password text not null,
                    user_name text not null,
                    role user_role not null 
                );

                create table accounts
                (
                    id bigint primary key generated always as identity,
                    pin text not null,
                    user_id bigint not null,
                    balance bigint not null
                );

                create table history 
                (
                    id bigint primary key generated always as identity,
                    account_id bigint not null,
                    amount bigint,
                    timestamp timestamp not null
                );
            """;

        context.Expressions.Add(new ExecuteSqlStatementExpression() { SqlStatement = sql });
    }

    public void GetDownExpressions(IMigrationContext context)
    {
        ArgumentNullException.ThrowIfNull(context);
        const string sql =
            """
                drop table users;
                drop table accounts;
                drop table history;
                drop type user_role;
            """;

        context.Expressions.Add(new ExecuteSqlStatementExpression() { SqlStatement = sql });
    }
}