# Crowd Simulation of Predators and Preys in Packs
> A crowd simulation of predator-prey interaction made in Unreal Engine 4.20

This simulation prioritises the motion of prey and predators during a hunt. For more detail, please check out the PDF file in the same directory.

___

## Pre-requisites:
___

- Unreal Engine 4.20.x with working C++ setup
- Ideally, Visual Studio 2017

To start a session, click play in the Unreal Engine editor.

Basic parameters of the smulation (such as number of agents, their velocity and force gain limits)
Most of the internal properties are editable in source.

### To generate your own agent groups: 
___

- Place blueprint classes for PreyPack and PredatorPack into the editor scene.  
- Insert properties displayed in the property editor.
- Set m_auto from the existing SimWorld blueprint pawn to false.
- Ensure both groups are aligned in the Z-axis.
- Open Level Blueprint.
- Highlight the PreyPack Blueprint class by selecting the object in the scene or the world outliner.
- Right-click in the level blueprint, select "Create Reference to [The name of the PreyPack]".
- Repeat for PredatorPack.
#### Now we have both references of the pack in the level blueprint
- In the level blueprint, after the Possess node in the BeginPlay event, drag out the InitSim node from the SimWorld reference and insert your Pack Reference into the correct pins.
- Remove the above modification or duplicate the level to switch to auto-generate.

### Building the source
___
- You can choose to click Compile in the UE4 editor
- Or right-click+click Build on the SimTech_Crowd project root in the class/directory outliner in Visual Studio.
- To access Visual Studio project before a .sln file is in the project root directory, either choose "Generate Visual Studio Project" from the Engine Editor's File tab, or in File Explorer, right-click the .uproject file and choose to "generate Visual Studio Project files".
