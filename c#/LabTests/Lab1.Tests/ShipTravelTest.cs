using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships.DualEngineShips;
using Itmo.ObjectOrientedProgramming.Lab1.Models;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;
using Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;
using Itmo.ObjectOrientedProgramming.Lab1.Services;
using Xunit;

namespace Itmo.ObjectOrientedProgramming.Lab1.Tests;

public class ShipTravelTest
{
    [Fact]
    public void HighDensityNebulaeMediocreDistanceTest()
    {
        var shuttle = new Shuttle();
        var avgur = new Avgur();

        var route = new Route(new Path(new HighDensityNebulae(), DefaultEnvironmentProps.MediumDistance));

        var travelRunner = new TravelRunner();

        travelRunner.Run(route, shuttle);
        travelRunner.Run(route, avgur);

        Assert.NotEqual(TravelStatus.Success, shuttle.Report.Status);
        Assert.NotEqual(TravelStatus.Success, avgur.Report.Status);
    }

    [Fact]
    public void HighDensityNebulaeMediocreDistanceWithExplosionTest()
    {
        var vaklas = new Vaklas();
        var photonicVaklas = new Vaklas(true);

        var path = new Path(new HighDensityNebulae(new Explosion()), DefaultEnvironmentProps.MediumDistance);
        var route = new Route(path);

        var runner = new TravelRunner();

        runner.Run(route, vaklas);
        runner.Run(route, photonicVaklas);

        Assert.Equal(TravelStatus.CrewDeath, vaklas.Report.Status);
        Assert.Equal(TravelStatus.Success, photonicVaklas.Report.Status);
    }

    [Fact]
    public void NitrinoNebulaeWithSpaceWhalesTest()
    {
        var vaklas = new Vaklas();
        var avgur = new Avgur();
        var meredian = new Meredian();

        var path = new Path(new NitrinoNebulae(new SpaceWhale()), DefaultEnvironmentProps.MediumDistance);
        var route = new Route(path);

        var runner = new TravelRunner();
        runner.Run(route, vaklas);
        runner.Run(route, avgur);
        runner.Run(route, meredian);

        Assert.Equal(TravelStatus.Destroy, vaklas.Report.Status);
        Assert.False(avgur.Deflector.IsAlive);
        Assert.Equal(TravelStatus.Success, meredian.Report.Status);
    }

    [Fact]
    public void ShortDistanceRouteInSpaceBestShipTest()
    {
        var shuttle = new Shuttle();
        var vaklas = new Vaklas();

        var route = new Route(new Path(new Space(), DefaultEnvironmentProps.ShortDistance));

        var runner = new TravelRunner();
        ShipBase bestShip = runner.GetBestShip(route, new List<ShipBase>() { shuttle, vaklas });

        Assert.Equal(bestShip, shuttle);
    }

    [Fact]
    public void HighDensityNebulaeMediocreDistanceBestShipTest()
    {
        var avgur = new Avgur();
        var stella = new Stella();

        var route = new Route(new Path(new HighDensityNebulae(), DefaultEnvironmentProps.MediumDistance));

        var runner = new TravelRunner();
        ShipBase bestShip = runner.GetBestShip(route, new List<ShipBase>() { avgur, stella });

        Assert.Equal(TravelStatus.Success, stella.Report.Status);
        Assert.False(avgur.Report.Status == TravelStatus.Success);
        Assert.Equal(stella, bestShip);
    }

    [Fact]
    public void NitrinoNebulaeTest()
    {
        var shuttle = new Shuttle();
        var vaklas = new Vaklas();

        var route = new Route(new Path(new NitrinoNebulae(), DefaultEnvironmentProps.MediumDistance));

        var runner = new TravelRunner();
        ShipBase bestShip = runner.GetBestShip(route, new List<ShipBase>() { shuttle, vaklas });

        Assert.Equal(vaklas, bestShip);
    }
}