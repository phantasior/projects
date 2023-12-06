using System.Collections.Generic;

namespace Itmo.ObjectOrientedProgramming.Lab2.Models;

public static class PredefinedSpecifications
{
    public static Specification GetFastCorrectSpecification
        => new(
            cpuName: "Intel Core i3-10105F CM8070104291323",
            motherBoard: "Gigabyte B550 AORUS ELITE V2 Lga1200",
            graphicCard: "Gigabyte GeForce RTX 4060 Ti GAMING OC 8Gb (GV-N406TGAMING OC-8GD)",
            ramSticks: new List<string>()
            {
                "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
                "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
            },
            dataStorage: "HDD 1Tb Samsung 970 Evo Plus NVMe M.2 (MZ-V7S1T0BW)",
            cpuCoolingSystem: "Be Quiet Dark Rock Pro 4 (BK022)",
            powerSupply: "700W Be Quiet System Power 9 Bronze (BN248)",
            computerCase: "Zalman i3 Edge Black");

    public static Specification GetSpecificationWithLackOfPowerInPowerSupply
        => new(
            cpuName: "Intel Core i3-10105F CM8070104291323",
            motherBoard: "Gigabyte B550 AORUS ELITE V2 Lga1200",
            graphicCard: "Gigabyte GeForce RTX 4060 Ti GAMING OC 8Gb (GV-N406TGAMING OC-8GD)",
            ramSticks: new List<string>()
            {
                "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
                "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
            },
            dataStorage: "HDD 1Tb Samsung 970 Evo Plus NVMe M.2 (MZ-V7S1T0BW)",
            cpuCoolingSystem: "Be Quiet Dark Rock Pro 4 (BK022)",
            powerSupply: "300W Be Quiet System Power 9 Bronze (BN248)",
            computerCase: "Zalman i3 Edge Black");

    public static Specification GetCorrectSpecificationWithCpuCoolingSystemWarning
        => new(
            cpuName: "Extra TPD Intel Core i3-10105F CM8070104291323",
            motherBoard: "Gigabyte B550 AORUS ELITE V2 Lga1200",
            graphicCard: "Gigabyte GeForce RTX 4060 Ti GAMING OC 8Gb (GV-N406TGAMING OC-8GD)",
            ramSticks: new List<string>()
            {
                "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
                "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
            },
            dataStorage: "HDD 1Tb Samsung 970 Evo Plus NVMe M.2 (MZ-V7S1T0BW)",
            cpuCoolingSystem: "Be Quiet Dark Rock Pro 4 (BK022)",
            powerSupply: "700W Be Quiet System Power 9 Bronze (BN248)",
            computerCase: "Zalman i3 Edge Black");
}