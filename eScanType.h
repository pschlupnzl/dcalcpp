#ifndef ESCANTYPE_H
#define ESCANTYPE_H

enum eScanType {
  SCAN_UNDEFINED,
  /** Input number only. */
  SCAN_NUMBER,
  /** Input binary operator. */
  SCAN_BINARYOP,
  /** Open parenthesis -(-. */
  SCAN_OPEN,
  /** Close parenthesis -)-. */
  SCAN_CLOSE
};

#endif /* ESCANTYPE_H */