using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;
using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Models.MotherBoardModels;
using Itmo.ObjectOrientedProgramming.Lab2.Models.СpuModels;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class ComponentsDataBase
{
    public ComponentsDataBase()
    {
        LoadComputerCase();
        LoadCpuCoolingSystem();
        LoadBios();
        LoadCpu();
        LoadGpu();
        LoadDataStorage();
        LoadMotherBoard();
        LoadPowerSupply();
        LoadWifiAdapter();
        LoadRamStick();
        LoadXmpProfile();
    }

    public ComponentsFactory GetComponentsFactory => new ComponentsFactory(
        new SpecificComponentFactory<Cpu>(CpuList),
        new SpecificComponentFactory<Bios>(BiosList),
        new SpecificComponentFactory<ComputerCase>(ComputerCaseList),
        new SpecificComponentFactory<CpuCoolingSystem>(CpuCoolingSystemList),
        new SpecificComponentFactory<DataStorageBase>(DataStorageList),
        new SpecificComponentFactory<GraphicCard>(GraphicCardList),
        new SpecificComponentFactory<MotherBoard>(MotherBoardList),
        new SpecificComponentFactory<PowerSupply>(PowerSupplyList),
        new SpecificComponentFactory<RamStick>(RamStickList),
        new SpecificComponentFactory<WifiAdapter>(WifiAdapterList),
        new SpecificComponentFactory<XmpProfile>(XmpProfileList));

    public IList<ComputerCase> ComputerCaseList { get; } = new List<ComputerCase>();
    public IList<Bios> BiosList { get; } = new List<Bios>();
    public IList<CpuCoolingSystem> CpuCoolingSystemList { get; } = new List<CpuCoolingSystem>();
    public IList<Cpu> CpuList { get; } = new List<Cpu>();
    public IList<DataStorageBase> DataStorageList { get; } = new List<DataStorageBase>();
    public IList<GraphicCard> GraphicCardList { get; } = new List<GraphicCard>();
    public IList<MotherBoard> MotherBoardList { get; } = new List<MotherBoard>();
    public IList<PowerSupply> PowerSupplyList { get; } = new List<PowerSupply>();
    public IList<RamStick> RamStickList { get; } = new List<RamStick>();
    public IList<WifiAdapter> WifiAdapterList { get; } = new List<WifiAdapter>();
    public IList<XmpProfile> XmpProfileList { get; } = new List<XmpProfile>();

    private void LoadComputerCase()
    {
        ComputerCaseList.Add(new ComputerCase(
            name: "Zalman i3 Edge Black",
            maxGpuHeight: 350,
            maxGpuWidth: 150,
            width: 500,
            height: 1200));
    }

    private void LoadCpuCoolingSystem()
    {
        CpuCoolingSystemList.Add(new CpuCoolingSystem(
            name: "Be Quiet Dark Rock Pro 4 (BK022)",
            width: 147,
            height: 136,
            maxTdp: 250,
            supportedSockets: new List<Sockets>() { Sockets.Lga1200, Sockets.Lga1700 }));
    }

    private void LoadCpu()
    {
        CpuList.Add(new Cpu(
            name: "Intel Core i3-10105F CM8070104291323",
            coreFrequency: 1200,
            coreAmount: 4,
            socket: Sockets.Lga1200,
            hasVideoCore: true,
            supportedFrequency: new List<int> { 2113 },
            tpd: 13,
            power: 65));

        CpuList.Add(new Cpu(
            name: "Extra TPD Intel Core i3-10105F CM8070104291323",
            coreFrequency: 1200,
            coreAmount: 4,
            socket: Sockets.Lga1200,
            hasVideoCore: true,
            supportedFrequency: new List<int> { 2113 },
            tpd: 350,
            power: 65));
    }

    private void LoadDataStorage()
    {
        DataStorageList.Add(new HddStorage(
            name: "HDD 1Tb Samsung 970 Evo Plus NVMe M.2 (MZ-V7S1T0BW)",
            capacity: 1000,
            powerSupply: 5,
            shindleSpeed: 7200));

        DataStorageList.Add(new SsdStorage(
            name: "SSD 1Tb Samsung 970 Evo Plus NVMe M.2 (MZ-V7S1T0BW)",
            capacity: 1000,
            powerSupply: 5,
            connectionType: SsdConnectionType.Sata,
            maxSpeed: 300));
    }

    private void LoadGpu()
    {
        GraphicCardList.Add(new GraphicCard(
            name: "Gigabyte GeForce RTX 4060 Ti GAMING OC 8Gb (GV-N406TGAMING OC-8GD)",
            height: 329,
            width: 133,
            memoryAmount: 12,
            pcieVersion: 4,
            chipFrequency: 2310,
            powerConsumption: 285));
    }

    private void LoadMotherBoard()
    {
        MotherBoardList.Add(new MotherBoard(
            name: "Gigabyte B550 AORUS ELITE V2 Lga1200",
            socket: Sockets.Lga1200,
            pcieLinesAmount: 4,
            sataPortsAmount: 6,
            cpuSlotAmount: 4,
            MotherBoardFormFactor.Atx,
            ddrVersion: DdrStandard.Ddr4,
            chipSet: new ChipSet(
                new[] { 12, 13 },
                true),
            hasInternalWifi: true));
    }

    private void LoadPowerSupply()
    {
        PowerSupplyList.Add(new PowerSupply(
            name: "700W Be Quiet System Power 9 Bronze (BN248)",
            maxPower: 700));

        PowerSupplyList.Add(new PowerSupply(
            name: "300W Be Quiet System Power 9 Bronze (BN248)",
            maxPower: 100));
    }

    private void LoadRamStick()
    {
        RamStickList.Add(new RamStick(
            name: "16Gb 3200MHz Kingston Fury Beast Black 2x8Gb KIT CL16 DDR4 (KF432C16BBK2/16)",
            memorySize: 16,
            power: 140,
            availableProfiles: new[] { "DOCP", "XMP", "A-XMP" },
            ddrVersion: DdrStandard.Ddr4,
            formFactor: RamFormFactor.Dimm,
            supportedFrequencies: new[] { 3200, 2113, 3223 }));
    }

    private void LoadBios()
    {
        BiosList.Add(new Bios("Bios", 12, 1, CpuList));
    }

    private void LoadWifiAdapter()
    {
        WifiAdapterList.Add(new WifiAdapter(
            "Wifi Adapter",
            WifiStandards.Ieee80211,
            true,
            4,
            5));
    }

    private void LoadXmpProfile()
    {
        XmpProfileList.Add(new XmpProfile("13-13-14-14", 28, 1044, "Xmp profile"));
    }
}