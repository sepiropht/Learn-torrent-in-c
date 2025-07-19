# BitTorrent Protocol Specification

## Overview

BitTorrent is a peer-to-peer (P2P) file sharing protocol that distributes large files efficiently across multiple peers. Instead of downloading from a single server, clients download pieces of files from many peers simultaneously.

## Protocol Components

### 1. Metainfo File (.torrent)

A .torrent file contains metadata about the files to be shared and the tracker information.

**Structure (Bencode dictionary):**
```
{
  "announce": "<tracker URL>",
  "info": {
    "name": "<file/directory name>",
    "piece length": <integer>,
    "pieces": "<SHA1 hashes concatenated>",
    "length": <integer> (single file mode),
    "files": [ (multi-file mode)
      {
        "path": ["<dir>", "<filename>"],
        "length": <integer>
      }
    ]
  }
}
```

**Key Fields:**
- `announce`: Primary tracker URL
- `info.name`: Suggested name for the file/directory
- `info.piece length`: Number of bytes per piece (typically 2^18 to 2^22)
- `info.pieces`: SHA1 hashes of each piece (20 bytes each)
- `info.length`: Total file size (single file torrents)
- `info.files`: List of files with paths and sizes (multi-file torrents)

**Info Hash:** SHA1 hash of the bencoded `info` dictionary. This uniquely identifies the torrent.

### 2. Tracker Protocol

Trackers maintain lists of peers participating in the torrent swarm.

#### HTTP Tracker Request

**URL Format:**
```
http://tracker.example.com/announce?info_hash=<hash>&peer_id=<id>&port=<port>&uploaded=<bytes>&downloaded=<bytes>&left=<bytes>&event=<event>
```

**Parameters:**
- `info_hash`: URL-encoded 20-byte SHA1 hash of info dictionary
- `peer_id`: 20-byte string identifying the client
- `port`: TCP port for incoming peer connections
- `uploaded`: Total bytes uploaded this session
- `downloaded`: Total bytes downloaded this session  
- `left`: Bytes remaining to download (0 when complete)
- `event`: `started`, `completed`, `stopped`, or omitted

**Optional Parameters:**
- `compact`: Set to 1 for compact peer list format
- `no_peer_id`: Set to 1 to omit peer IDs from response
- `numwant`: Number of peers to return (default 50)

#### Tracker Response

**Success Response (Bencode dictionary):**
```
{
  "interval": <seconds>,
  "peers": <peer list>,
  "complete": <integer> (optional),
  "incomplete": <integer> (optional)
}
```

**Peer List Formats:**
1. **Dictionary format:**
   ```
   [
     {"peer id": "<20 bytes>", "ip": "<IP>", "port": <port>},
     ...
   ]
   ```

2. **Compact format:**
   ```
   "<6 bytes per peer: 4 byte IP + 2 byte port>"
   ```

**Error Response:**
```
{
  "failure reason": "<error message>"
}
```

### 3. Peer Wire Protocol

Communication between BitTorrent clients uses TCP connections.

#### Handshake

**Format:**
```
<pstrlen><pstr><reserved><info_hash><peer_id>
```

- `pstrlen`: 1 byte = 19 (length of protocol string)
- `pstr`: 19 bytes = "BitTorrent protocol"
- `reserved`: 8 bytes (all zeros, used for extensions)
- `info_hash`: 20 bytes SHA1 hash
- `peer_id`: 20 bytes peer identifier

#### Messages

All messages after handshake have format:
```
<length prefix><message ID><payload>
```

- `length prefix`: 4-byte big-endian integer
- `message ID`: 1 byte
- `payload`: Variable length

**Message Types:**

| ID | Name | Payload | Description |
|----|------|---------|-------------|
| - | keep-alive | none | len=0, no ID |
| 0 | choke | none | Stop sending requests |
| 1 | unchoke | none | Resume sending requests |
| 2 | interested | none | Want to download |
| 3 | not interested | none | Don't want to download |
| 4 | have | piece index (4 bytes) | Announce piece availability |
| 5 | bitfield | bitfield | Announce all available pieces |
| 6 | request | index, begin, length (12 bytes) | Request piece block |
| 7 | piece | index, begin, block (9+ bytes) | Send piece block |
| 8 | cancel | index, begin, length (12 bytes) | Cancel request |

#### Connection State

Each peer connection maintains state:
- **Choked/Unchoked**: Whether peer will send pieces
- **Interested/Not Interested**: Whether peer wants pieces
- **Bitfield**: Which pieces each peer has

Initial state: choked and not interested.

### 4. Piece Management

#### Piece Selection Strategies

1. **Random First Piece**: Select random piece to start
2. **Rarest First**: Select pieces owned by fewest peers
3. **Endgame Mode**: When few pieces remain, request from multiple peers

#### Piece Verification

1. Download all blocks for a piece
2. Concatenate blocks in order
3. Calculate SHA1 hash
4. Compare with hash from .torrent file
5. If mismatch, re-download the piece

#### Block Requests

Pieces are divided into blocks (typically 16KB):
- Request multiple blocks simultaneously (pipelining)
- Typical pipeline depth: 5-10 outstanding requests
- Cancel requests from slow peers

### 5. Client Algorithms

#### Choking Algorithm

**Purpose**: Optimize upload bandwidth and encourage reciprocation

**Standard Algorithm:**
1. Unchoke 3-4 fastest downloaders (reciprocal unchoke)
2. Unchoke 1 random peer every 30 seconds (optimistic unchoke)
3. Rechoke every 10 seconds based on download rates

#### Peer Discovery

**Methods:**
1. **Tracker**: Get peers from HTTP/UDP tracker
2. **DHT**: Distributed Hash Table (BEP 5)
3. **PEX**: Peer Exchange (BEP 11)
4. **Local Discovery**: Multicast on LAN

#### Connection Management

- **Maximum connections**: Typically 50-200 peers
- **Connection timeout**: 20-30 seconds for handshake
- **Keep-alive**: Send every 2 minutes
- **Bandwidth allocation**: Balance upload/download

### 6. File Assembly

#### Single File Torrents
- Map piece data directly to file offset
- Create file with correct size
- Write piece data at calculated offsets

#### Multi-File Torrents
- Calculate which files each piece spans
- Create directory structure
- Distribute piece data across multiple files
- Handle pieces that span file boundaries

### 7. Protocol Extensions

#### Fast Extension (BEP 6)
- **have_all**: Announce having all pieces
- **have_none**: Announce having no pieces
- **suggest_piece**: Suggest piece to download
- **reject_request**: Decline piece request
- **allowed_fast**: Allow requests even when choked

#### DHT (BEP 5)
- Distributed peer discovery without trackers
- Based on Kademlia distributed hash table
- Peers store location information for torrents

#### Peer Exchange (BEP 11)
- Share peer lists between connected peers
- Reduce tracker load
- Faster peer discovery

### 8. Implementation Phases

#### Phase 1: Metainfo Parsing
1. Parse .torrent files (Bencode)
2. Extract info hash, piece hashes, file info
3. Validate metainfo structure

#### Phase 2: Tracker Communication
1. Build tracker announce URLs
2. Send HTTP requests to trackers
3. Parse tracker responses
4. Handle tracker errors and retries

#### Phase 3: Peer Wire Protocol
1. Establish TCP connections to peers
2. Perform BitTorrent handshake
3. Exchange messages (bitfield, interested, etc.)
4. Handle connection state

#### Phase 4: Piece Download
1. Select pieces to download
2. Request blocks from peers
3. Assemble and verify pieces
4. Handle piece failures and retries

#### Phase 5: File Assembly
1. Map pieces to file offsets
2. Create output files and directories
3. Write piece data to correct locations
4. Handle incomplete downloads

#### Phase 6: Seeding
1. Respond to peer requests
2. Upload pieces to other peers
3. Implement choking algorithms
4. Maintain good swarm participation

### 9. Error Handling

#### Common Errors
- **Invalid metainfo**: Malformed .torrent files
- **Tracker failures**: HTTP errors, invalid responses
- **Peer disconnects**: Network issues, protocol violations
- **Piece corruption**: Hash mismatches, network errors
- **Disk errors**: Write failures, insufficient space

#### Recovery Strategies
- **Retry mechanisms**: Exponential backoff for failed operations
- **Fallback trackers**: Try backup trackers from announce-list
- **Piece re-download**: Request corrupted pieces from different peers
- **Connection replacement**: Drop slow peers, find new ones

### 10. Security Considerations

#### Hash Verification
- Always verify piece hashes before accepting
- Reject peers sending invalid data
- Protect against hash collision attacks

#### Rate Limiting
- Limit connection attempts per IP
- Throttle requests to prevent DoS
- Implement upload/download limits

#### Privacy
- Use random peer IDs
- Consider proxy/VPN usage
- Be aware of IP address exposure

## References

- **BEP 3**: The BitTorrent Protocol Specification
- **BEP 5**: DHT Protocol
- **BEP 6**: Fast Extension
- **BEP 11**: Peer Exchange (PEX)
- **BEP 15**: UDP Tracker Protocol