# Custom metadata

<!-- References -->
[wsdp]: https://specs.xmlsoap.org/ws/2006/02/devprof/devicesprofile.pdf
[pnpx]: https://download.microsoft.com/download/a/f/7/af7777e5-7dcd-4800-8a0a-b18336565f5b/PnPX-spec.doc
[ms-pbsd]: https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-pbsd/a3c6b665-a44e-41d5-98ec-d70c188378e4
[ms-dpwsrp]: https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dpwsrp/0a96c35a-4cd4-4274-9f42-f44334d4d893
[cont-id]: https://learn.microsoft.com/en-us/windows-hardware/drivers/install/container-ids-for-dpws-devices
[metadata-retrieval]: https://learn.microsoft.com/en-us/windows-hardware/drivers/install/device-metadata-retrieval-client
[wsdd-print]: https://learn.microsoft.com/en-us/windows-hardware/drivers/print/ws-discovery-mobile-printing-support
<!-- End References -->

WS-Discovery protocol tells Windows what kind of device it is exposing by sending over "metadata" - essentially an XML document containing information about the computer.

By default **wsdd-native** sends metadata describing the host it is running on as an SMB server. It is possible to change that by authoring your own metadata and telling **wsdd-native** to use that instead. 

To do so you need to use `--metadata PATH` or `-m PATH` command line switch or put `metadata="path"` in `wsddn.conf` file.

## Format

Custom metadata must be a valid, well-formed, standalone XML file. All namespaces you use must be fully declared. 

### General form

The general form of the metadata is as follows. For more details see [this obscure spec][wsdp]

```xml
<?xml version="1.0" encoding="UTF-8"?>
<wsx:Metadata 
    xmlns:wsx="http://schemas.xmlsoap.org/ws/2004/09/mex"
    xmlns:wsdp="http://schemas.xmlsoap.org/ws/2006/02/devprof"
    xmlns:pnpx="http://schemas.microsoft.com/windows/pnpx/2005/10" >
  <wsx:MetadataSection Dialect="http://schemas.xmlsoap.org/ws/2006/02/devprof/ThisDevice">
    <wsdp:ThisDevice>
        <wsdp:FriendlyName>...</wsdp:FriendlyName>
        <wsdp:FirmwareVersion>...</wsdp:FirmwareVersion>
        <wsdp:SerialNumber>...</wsdp:SerialNumber>
    </wsdp:ThisDevice>
  </wsx:MetadataSection>
  <wsx:MetadataSection Dialect="http://schemas.xmlsoap.org/ws/2006/02/devprof/ThisModel">
    <wsdp:ThisModel>
      <wsdp:Manufacturer>...</wsdp:Manufacturer>
      <wsdp:ModelName>...</wsdp:ModelName>
      <wsdp:ModelNumber>...</wsdp:ModelNumber>
      <pnpx:DeviceCategory>...</pnpx:DeviceCategory>
    </wsdp:ThisModel>
  </wsx:MetadataSection>
  <wsx:MetadataSection Dialect="http://schemas.xmlsoap.org/ws/2006/02/devprof/Relationship">
    <wsdp:Relationship Type="http://schemas.xmlsoap.org/ws/2006/02/devprof/host">
    ...
    </wsdp:Relationship>
  </wsx:MetadataSection>
</wsx:Metadata>
```

### Placeholders

You can use _placeholders_ inside XML element values and attributes. The placeholders are keywords that start with a `$` sign.

The rules for placeholders are as follows:

* A single `$` not followed by a placeholder is simply dropped and ignored
* `$$` is replaced with a single `$`
* The following placeholders are currently defined:

| Placeholder           | Meaning 
|-----------------------|--------
| $ENDPOINT_ID          | Replaced with the URN in the form urn:uuid:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx where the UUID is the identifier of the host. It is auto-generated or supplied via `--uuid` option.
| $IP_ADDR              | The IP address of the interface on which the metadata is being sent. This allows you to create URLs that operate on the same network Windows that Windows "sees" your machine.
| $SMB_HOST_DESCRIPTION | The description of the host as derived from SMB configuration. If no description is available simple SMB host name is used.
| $SMB_FULL_HOST_NAME   | A string in format `hostname/Workgroup:workgroup_name` or `hostname/Domain:domain_name` that provides full SMB name of the machine in WS-Discovery protocol.

* Placeholders are _prefix-matched_ so $IP_ADDR_HELLO will be expanded to something like 192.168.1.1_HELLO

### Examples

This directory contains some examples that might help you author your own metadata. 

The [default.xml](default.xml) file contains an equivalent of what **wsdd-native** sends by default with no custom metadata. This is the standard metadata of an SMB host. 

The [other.xml](other.xml) file contains an example of a simple HTTP server. When you click on it in Windows Explorer the browser would open pointing to that host. Where it points to is actually controlled by the line

```xml
<wsdp:PresentationUrl>http://$IP_ADDR/</wsdp:PresentationUrl>
```
which you can change to anything you want.

The section and icon in Windows Explorer Network view are determined by the line

```xml
<pnpx:DeviceCategory>Other</pnpx:DeviceCategory>
```

In the example it is `Other` which would result in the host being in "Other devices". You can try other things here like `HomeAutomation` or `MediaDevices`. The full(?) list of what is allowed here can be divined from [this obscure spec][pnpx] (**warning: .doc file**). See the table in "PnPX Category Definitions" section.


## More information

The entire area of what the actual WS-Discovery payload should be is incredibly poorly documented by Microsoft. The useful references (some already mentioned) I could find are as follows:

* [Devices Profile for Web Services][wsdp] - a horribly formatted obscure spec that leaves many questions unanswered
* [PnPX: Plug and Play Extensions for Windows][pnpx] - (**warning: .doc file**). A very old spec that seems to exist only as a Word document. This sheds some light on `pnpx` stuff that can be used in metadata.
* [\[MS-PBSD\]: Publication Services Data Structure][ms-pbsd] - see especially "Section 3: Structure Examples" there. This describes the metadata for an SMB server.
* [\[MS-DPWSRP\]: Devices Profile for Web Services (DPWS): Shared Resource Publishing Data Structure][ms-dpwsrp] - explains the bizarre "not quite base-64" data is in the "Structure Examples" above
* [Container IDs for DPWS Devices][cont-id] - explains how multiple devices can be grouped into "containers" 
* [Device Metadata Retrieval Client][metadata-retrieval] - indirectly explains how Windows looks up more information about devices including device-specific icons etc. Unfortunately it seems that there is no way to specify an icon via WS-Discovery, you need to register extra device metadata with Microsoft and refer to it by device ID.
* [WS-Discovery mobile printing support][wsdd-print] - an example of how printers should expose themselves via WS-Discovery



