/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2015, 2016, 2017, 2018, 2019 Teunis van Beelen
*
* Email: teuniz@protonmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/


//#include "mainwindow.h"
#include "connection.h"
#include "osc_lan.h"

#define MAX_CMD_LEN     (255)
#define MAX_RESP_LEN    (1024 * 1024 * 2)

int tmc_connection_type;

struct tmcdev *tmc_device;

struct tmcdev * tmc_open_usb(const char *device)
{
  tmc_connection_type = 0;

  tmc_device = tmcdev_open(device);

  return tmc_device;
}


struct tmcdev * tmc_open_lan(const char *address)
{
  tmc_connection_type = 1;

  if(tmc_device) {
    tmc_close();
  }

  tmc_device = (struct tmcdev *)calloc(1, sizeof(struct tmcdev));
  if(tmc_device == nullptr) {
    return nullptr;
  }

  tmc_device->hdrbuf = (char *)calloc(1, MAX_RESP_LEN + 1024);
  if(tmc_device->hdrbuf == nullptr) {
    free(tmc_device);
    tmc_device = nullptr;
    return nullptr;
  }

  tmc_device->buf = tmc_device->hdrbuf;

  tmc_device->fd = ocs.open(address);

  if(tmc_device->fd < 0)  {
    free(tmc_device->hdrbuf);

    free(tmc_device);

    tmc_device = nullptr;
  }
  return tmc_device;
}


void tmc_close(void)
{
  if(tmc_connection_type == 0)
  {
    tmcdev_close(tmc_device);
  }
  else
  {
      if(tmc_device != nullptr)
      {

        ocs.close();

        free(tmc_device->hdrbuf);

        free(tmc_device);

      }
  }
  tmc_device = nullptr;
}


int tmc_write(const char *cmd)
{
    int i, n, len, qry=0;
    char buf[MAX_CMD_LEN + 16];
    char str[128];
    qDebug() << "tmc_write:" << cmd;
    if(tmc_device == nullptr || tmc_device->fd < 0)
    {
      qDebug("tmc_lan error: device = Null\n");
      return -1;
    }

    len = (int) strlen(cmd);

    if(len > MAX_CMD_LEN)
    {
      qDebug("tmc_lan error: command too long\n");

      return -1;
    }

    if(len < 2)
    {
      qDebug("tmc_lan error: command too short\n");

      return -1;
    }
    if(cmd[len - 1] == '?')
    {
      qry = 1;
    }
    strlcpy(buf, cmd, sizeof(buf));
    strlcat(buf, "\n", sizeof(buf));
#if 0
    if(!(!strncmp(buf, ":TRIG:STAT?", 11) ||  /* don't print these commands to the console */
        !strncmp(buf, ":TRIG:SWE?", 10) ||    /* because they are used repeatedly */
        !strncmp(buf, ":WAV:DATA?", 10) ||
        !strncmp(buf, ":WAV:MODE NORM", 14) ||
        !strncmp(buf, ":WAV:FORM BYTE", 14) ||
        !strncmp(buf, ":WAV:SOUR CHAN", 14) ||
        !strncmp(buf, ":TRIG:SWE?", 10) ||
        !strncmp(buf, ":ACQ:SRAT?", 10) ||
        !strncmp(buf, ":ACQ:MDEP?", 10) ||
        !strncmp(buf, ":MEAS:COUN:VAL?", 15) ||
        !strncmp(buf, ":FUNC:WREC:OPER?", 16) ||
        !strncmp(buf, ":FUNC:WREP:OPER?", 16) ||
        !strncmp(buf, ":FUNC:WREP:FMAX?", 16) ||
        !strncmp(buf, ":FUNC:WREC:FMAX?", 16) ||
        !strncmp(buf, ":FUNC:WREP:FCUR?", 16) ||
        !strncmp(buf, ":WAV:XOR?", 9)))
      qDebug("tmc_lan write: %s", buf);
#endif
  if(!strncmp(buf, "*RST", 4)) // || (!strncmp(buf, ":AUT", 4)))
  {
    qry = 1;
  }

  if(tmc_connection_type == 0)
  {
    return tmcdev_write(tmc_device, cmd);
  }
  else
  {

      //    return tmclan_write(tmc_device, cmd);
      n = ocs.write(buf, len + 1);

      if(n != (len + 1))
      {
        qDebug("tmclan error: device write error");

        return -1;
      }

      if(!qry)
      {
        for(i=0; i<20; i++)
        {
          qDebug() << "tmc_write: *OPC?";
          if(ocs.write("*OPC?\n", 6) != 6)
          {
            qDebug("tmclan error: device write error");

            return -1;
          }

          n = ocs.read(str, 128);
          qDebug() << "ocs.read() = " << n;

          if(n < 0)
          {
            qDebug("tmclan error: device read error");

            return -1;
          }

          if(n == 2)
          {
            if(str[0] == '1')
            {
              break;
            }
          }
          usleep(25000);
        }
      }

      return len;
  }
}

void tmc_flush(void) {
    if(tmc_connection_type && tmc_device != nullptr && tmc_device->fd != 0)
        ocs.flush();
}

int tmc_read(void)
{
    int n, size, size2, len;

    char blockhdr[32];
    if(tmc_device == nullptr || tmc_device->fd < 0)
    {
      return -1;
    }

  if(tmc_connection_type == 0)
  {
    return tmcdev_read(tmc_device);
  }
  else
  {

      tmc_device->hdrbuf[0] = 0;

      tmc_device->sz = 0;

      size = 0;

      while(1)
      {
        n = ocs.read(tmc_device->hdrbuf + size, MAX_RESP_LEN - size);
        qDebug() << "ocs.read() = " << n;
        if(n < 1)
        {
          return -2;
        }

        size += n;

        if(tmc_device->hdrbuf[size - 1] == '\n')
        {
          break;
        }
      }

      if((size < 2) || (size > MAX_RESP_LEN))
      {
        tmc_device->hdrbuf[0] = 0;

        tmc_device->buf[0] = 0;

        return -3;
      }

      tmc_device->hdrbuf[size] = 0;

      if(tmc_device->hdrbuf[0] != '#')
      {
        if(tmc_device->hdrbuf[size - 1] == '\n')
        {
          tmc_device->hdrbuf[--size] = 0;
        }

        tmc_device->buf = tmc_device->hdrbuf;

        tmc_device->sz = size;

        return tmc_device->sz;
      }

      strncpy(blockhdr, tmc_device->hdrbuf, 16);

      len = blockhdr[1] - '0';

      if((len < 1) || (len > 9))
      {
        blockhdr[31] = 0;

        return -1;
      }

      blockhdr[len + 2] = 0;

      size2 = atoi(blockhdr + 2);

      size--;  // remove the last character

      if(size < size2)
      {
        blockhdr[31] = 0;

        return -1;
      }

      tmc_device->buf = tmc_device->hdrbuf + len + 2;

      tmc_device->sz = size2;

      return tmc_device->sz;
  }
}
