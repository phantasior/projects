using System;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Services;

namespace Itmo.ObjectOrientedProgramming.Lab2;

public static class Program
{
    public static void Main()
    {
        var dataBase = new ComponentsDataBase();
        var configurator = new ComputerConfigurator(dataBase.GetComponentsFactory);
        Result<Computer, string> computer =
            configurator.BuildComputer(PredefinedSpecifications.GetCorrectSpecificationWithCpuCoolingSystemWarning);

        if (computer.HasValue)
        {
            Console.WriteLine("Successfully was built!");
            if (!computer.Value().Warnings.Any())
            {
                return;
            }

            Console.WriteLine("Warnings: ");
            computer.Value().Warnings.ToList().ForEach(Console.WriteLine);
        }
        else
        {
            Console.WriteLine("Error during building:");
            Console.WriteLine(computer.Error());
        }
    }
}